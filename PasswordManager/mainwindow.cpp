#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "databasemanager.h"
#include "passwordfilterproxymodel.h"
#include "passwordrepository.h"
#include "passwordtablemodel.h"

#include <QAbstractItemView>
#include <QDir>
#include <QHeaderView>
#include <QMessageBox>
#include <QStandardPaths>
#include <QStatusBar>
#include <QDateTime>
#include "passwordleakchecker.h"
#include <QAction>
#include <QProgressDialog>

#include <QtConcurrent>
#include <QFutureWatcher>
#include "passwordbatch.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_dbManager(new DatabaseManager)
{
    ui->setupUi(this);

    ui->labelNoResults->setVisible(false);

    if (!initDatabase()) {
        return;
    }

    initModel();
    initConnections();
    initSecurityChecker();
    reloadTable();

    ui->tableEntries->horizontalHeader()->setStretchLastSection(true);
    ui->tableEntries->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableEntries->verticalHeader()->setVisible(false);
    ui->tableEntries->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableEntries->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableEntries->setEditTriggers(
        QAbstractItemView::DoubleClicked |
        QAbstractItemView::EditKeyPressed |
        QAbstractItemView::SelectedClicked
        );
    ui->tableEntries->setAlternatingRowColors(true);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_dbManager;
}

bool MainWindow::initDatabase()
{
    const QString appDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(appDir);

    const QString dbPath = appDir + "/passwordmanager.db";

    if (!m_dbManager->open(dbPath)) {
        showError("Database error", m_dbManager->lastError());
        return false;
    }

    if (!m_dbManager->initializeSchema()) {
        showError("Schema error", m_dbManager->lastError());
        return false;
    }

    m_repository = std::make_unique<PasswordRepository>(m_dbManager->database());
    return true;
}

void MainWindow::initModel()
{
    m_model = new PasswordTableModel(m_repository.get(), this);
    m_proxy = new PasswordFilterProxyModel(this);
    m_proxy->setSourceModel(m_model);

    ui->tableEntries->setModel(m_proxy);
    ui->tableEntries->setColumnHidden(PasswordTableModel::CheckColumn, false);
}

void MainWindow::initConnections()
{
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::onNewClicked);
    connect(ui->actionEdit, &QAction::triggered, this, &MainWindow::onEditClicked);
    connect(ui->actionDelete, &QAction::triggered, this, &MainWindow::onDeleteClicked);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onSaveClicked);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onExitClicked);

    connect(ui->editSearch, &QLineEdit::textChanged, this, &MainWindow::applyFilters);
    connect(ui->comboCategoryFilter, &QComboBox::currentTextChanged, this, &MainWindow::applyFilters);
    connect(ui->btnClearSearch, &QPushButton::clicked, this, &MainWindow::clearFilters);
}

void MainWindow::reloadTable()
{
    QString error;
    if (!m_model->load(&error)) {
        showError("Load error", error);
        return;
    }

    refreshCategoryFilter();
    applyFilters();
    updateEmptyState();
}

void MainWindow::refreshCategoryFilter()
{
    const QString current = ui->comboCategoryFilter->currentText();

    ui->comboCategoryFilter->blockSignals(true);
    ui->comboCategoryFilter->clear();
    ui->comboCategoryFilter->addItem("All");

    const QStringList cats = m_model->categories();
    for (const QString &cat : cats) {
        if (!cat.trimmed().isEmpty()) {
            ui->comboCategoryFilter->addItem(cat.trimmed());
        }
    }

    const int idx = ui->comboCategoryFilter->findText(current);
    ui->comboCategoryFilter->setCurrentIndex(idx >= 0 ? idx : 0);
    ui->comboCategoryFilter->blockSignals(false);
}

void MainWindow::applyFilters()
{
    m_proxy->setSearchText(ui->editSearch->text());
    m_proxy->setCategoryFilter(ui->comboCategoryFilter->currentText());
    updateEmptyState();
}

void MainWindow::clearFilters()
{
    ui->editSearch->clear();
    ui->comboCategoryFilter->setCurrentIndex(0);
    applyFilters();
}

void MainWindow::updateEmptyState()
{
    const bool empty = (m_proxy->rowCount() == 0);
    ui->labelNoResults->setVisible(empty);
}

void MainWindow::onNewClicked()
{
    QString error;

    PasswordRecord record;
    record.title = "New entry";
    record.updatedAt = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    if (!m_repository->insert(record, &error)) {
        showError("Insert error", error);
        return;
    }

    reloadTable();
}

void MainWindow::onEditClicked()
{
    const QModelIndex proxyIndex = ui->tableEntries->currentIndex();
    if (!proxyIndex.isValid())
        return;

    QModelIndex editIndex = proxyIndex;
    if (proxyIndex.column() == PasswordTableModel::IdColumn) {
        editIndex = proxyIndex.sibling(proxyIndex.row(), PasswordTableModel::TitleColumn);
    }

    ui->tableEntries->edit(editIndex);
}

void MainWindow::onDeleteClicked()
{
    const QModelIndex proxyIndex = ui->tableEntries->currentIndex();
    if (!proxyIndex.isValid())
        return;

    const auto answer = QMessageBox::question(
        this,
        "Delete entry",
        "Delete selected record?"
        );

    if (answer != QMessageBox::Yes)
        return;

    const QModelIndex sourceIndex = m_proxy->mapToSource(proxyIndex);

    QString error;
    if (!m_model->deleteRow(sourceIndex.row(), &error)) {
        showError("Delete error", error);
        return;
    }

    reloadTable();
}

void MainWindow::onSaveClicked()
{
    reloadTable();
}

void MainWindow::onExitClicked()
{
    close();
}

void MainWindow::showError(const QString &title, const QString &message)
{
    QMessageBox::critical(this, title, message);
}

void MainWindow::initSecurityChecker()
{
    m_passwordLeakChecker = new PasswordLeakChecker(this);

    m_checkPasswordAction = new QAction("Check Password", this);
    ui->menuTools->addAction(m_checkPasswordAction);
    ui->mainToolBar->addAction(m_checkPasswordAction);

    connect(m_checkPasswordAction, &QAction::triggered, this, &MainWindow::onCheckPasswordClicked);

    m_checkAllAction = new QAction("Check All Passwords", this);
    ui->menuTools->addAction(m_checkAllAction);
    ui->mainToolBar->addAction(m_checkAllAction);

    connect(m_checkAllAction, &QAction::triggered, this, &MainWindow::onCheckAllPasswords);

    m_checkPasswordDialog = new QProgressDialog("Checking password...", QString(), 0, 0, this);
    m_checkPasswordDialog->setWindowTitle("Password check");
    m_checkPasswordDialog->setCancelButton(nullptr);
    m_checkPasswordDialog->setWindowModality(Qt::NonModal);
    m_checkPasswordDialog->setMinimumDuration(300);
    m_checkPasswordDialog->hide();

    connect(m_passwordLeakChecker, &PasswordLeakChecker::checkStarted, this, [this]() {
        m_checkPasswordAction->setEnabled(false);
        m_checkPasswordDialog->show();
        statusBar()->showMessage("Checking password...");
    });

    connect(m_passwordLeakChecker, &PasswordLeakChecker::checkCompleted, this,
            [this](bool found, int count) {
                m_checkPasswordDialog->hide();
                m_checkPasswordAction->setEnabled(true);

                if (found) {
                    QMessageBox::warning(
                        this,
                        "Password check",
                        QString("Password was found in leaked databases %1 time(s).").arg(count)
                        );
                } else {
                    QMessageBox::information(
                        this,
                        "Password check",
                        "Password was not found in leaked databases."
                        );
                }

                statusBar()->showMessage("Password check finished", 3000);
            });

    connect(m_passwordLeakChecker, &PasswordLeakChecker::checkFailed, this,
            [this](const QString &message) {
                m_checkPasswordDialog->hide();
                m_checkPasswordAction->setEnabled(true);
                showError("Password check error", message);
                statusBar()->showMessage("Password check failed", 3000);
            });
    m_batchProgress = new QProgressDialog("Checking all passwords...", "Cancel", 0, 100, this);
    m_batchProgress->setWindowTitle("Batch check");
    m_batchProgress->setWindowModality(Qt::NonModal);
    m_batchProgress->setMinimumDuration(0);
    m_batchProgress->setAutoClose(false);
    m_batchProgress->hide();
}

void MainWindow::onCheckPasswordClicked()
{
    if (!m_passwordLeakChecker) {
        initSecurityChecker();
    }

    if (!m_model || !m_proxy) {
        return;
    }

    const QModelIndex proxyIndex = ui->tableEntries->currentIndex();
    if (!proxyIndex.isValid()) {
        showError("Password check", "Select a record first.");
        return;
    }

    const QModelIndex sourceIndex = m_proxy->mapToSource(proxyIndex);
    if (!sourceIndex.isValid()) {
        showError("Password check", "Unable to map selected row.");
        return;
    }

    const PasswordRecord &record = m_model->itemAt(sourceIndex.row());
    if (record.password.trimmed().isEmpty()) {
        showError("Password check", "Selected record has an empty password.");
        return;
    }

    m_passwordLeakChecker->checkPassword(record.password);
}

void MainWindow::onCheckAllPasswords()
{

    if (m_batchWatcher) {
        return;
    }

    m_checkAllAction->setEnabled(false);
    statusBar()->showMessage("Batch check started...");

    QVector<BatchItem> items;

    for (int row = 0; row < m_model->rowCount(); ++row)
    {
        const auto &rec = m_model->itemAt(row);

        if (!rec.password.trimmed().isEmpty())
            items.append({row, rec.password});
    }

    if (items.isEmpty()) {
        showError("Batch", "No passwords");
        return;
    }

    m_batchProgress->setMaximum(items.size());
    m_batchProgress->setValue(0);
    m_batchProgress->show();

    m_batchWatcher = new QFutureWatcher<QVector<BatchResult>>(this);

    QFuture<QVector<BatchResult>> future =
        QtConcurrent::run([this, items]() {
            return checkAllPasswords(items, [this](int current, int total) {

                QMetaObject::invokeMethod(this, [this, current, total]() {
                    if (m_batchProgress) {
                        m_batchProgress->setMaximum(total);
                        m_batchProgress->setValue(current);
                    }

                    statusBar()->showMessage(
                        QString("Checking %1 / %2").arg(current).arg(total)
                        );
                });

            });
        });

    connect(m_batchWatcher, &QFutureWatcher<QVector<BatchResult>>::finished,
            this, &MainWindow::onBatchFinished);

    m_batchWatcher->setFuture(future);
}

void MainWindow::onBatchFinished()
{
    auto results = m_batchWatcher->result();

    for (const auto &r : results)
    {
        QModelIndex idx = m_model->index(r.row, PasswordTableModel::CheckColumn);

        QString status;

        if (!r.result.error.isEmpty()) {
            status = "Error";
        }
        else if (r.result.found) {
            status = QString("Leaked (%1)").arg(r.result.count);
        }
        else {
            status = "Safe";
        }

        m_model->setData(idx, status, Qt::EditRole);
    }

    m_batchWatcher->deleteLater();
    m_batchWatcher = nullptr;

    if (m_batchProgress) {
        m_batchProgress->hide();
    }

    m_checkAllAction->setEnabled(true);
    statusBar()->showMessage("Batch check finished", 3000);
}
