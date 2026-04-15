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
