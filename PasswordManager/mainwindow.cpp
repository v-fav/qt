#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "databasemanager.h"
#include "passwordfilterproxymodel.h"
#include "passwordrepository.h"
#include "passwordtablemodel.h"

#include <QAbstractItemView>
#include <QDir>
#include <QFileInfo>
#include <QHeaderView>
#include <QMessageBox>
#include <QStandardPaths>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_dbManager(new DatabaseManager),
    m_model(nullptr),
    m_proxy(nullptr)
{
    ui->setupUi(this);

    initTable();

    if (!initDatabase()) {
        return;
    }

    initModel();
    initConnections();

    refreshCategoryFilter();
    applyFilters();

    statusBar()->showMessage("Database loaded", 2000);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_dbManager;
}

bool MainWindow::initDatabase()
{
    const QString appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(appDataDir);

    const QString dbPath = appDataDir + "/passwordmanager.db";

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

    QString error;
    if (!m_model->load(&error)) {
        showError("Load error", error);
    }

    ui->tableEntries->setModel(m_proxy);
    ui->tableEntries->setSortingEnabled(true);

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
}

void MainWindow::initTable()
{
    ui->tableEntries->setAlternatingRowColors(true);
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

void MainWindow::refreshCategoryFilter()
{
    const QString current = ui->comboCategoryFilter->currentText();

    ui->comboCategoryFilter->blockSignals(true);
    ui->comboCategoryFilter->clear();
    ui->comboCategoryFilter->addItem("All");

    if (m_model) {
        const QStringList cats = m_model->categories();
        for (const QString &cat : cats) {
            if (!cat.trimmed().isEmpty()) {
                ui->comboCategoryFilter->addItem(cat.trimmed());
            }
        }
    }

    int index = ui->comboCategoryFilter->findText(current);
    if (index < 0) {
        index = 0;
    }

    ui->comboCategoryFilter->setCurrentIndex(index);
    ui->comboCategoryFilter->blockSignals(false);
}

void MainWindow::applyFilters()
{
    if (!m_proxy) {
        return;
    }

    m_proxy->setSearchText(ui->editSearch->text());
    m_proxy->setCategoryFilter(ui->comboCategoryFilter->currentText());
}

void MainWindow::clearFilters()
{
    ui->editSearch->clear();
    ui->comboCategoryFilter->setCurrentIndex(0);
    applyFilters();
}

void MainWindow::selectLastRowAndEdit()
{
    if (!m_model || !m_proxy) {
        return;
    }

    const int sourceRow = m_model->rowCount() - 1;
    if (sourceRow < 0) {
        return;
    }

    QModelIndex sourceIndex = m_model->index(sourceRow, PasswordTableModel::TitleColumn);
    QModelIndex proxyIndex = m_proxy->mapFromSource(sourceIndex);

    if (proxyIndex.isValid()) {
        ui->tableEntries->setCurrentIndex(proxyIndex);
        ui->tableEntries->edit(proxyIndex);
    }
}

void MainWindow::onNewClicked()
{
    if (!m_model) {
        return;
    }

    const QString defaultCategory =
        ui->comboCategoryFilter->currentText() == "All"
            ? QString()
            : ui->comboCategoryFilter->currentText();

    m_model->addEmptyRow(defaultCategory);
    refreshCategoryFilter();
    applyFilters();
    selectLastRowAndEdit();

    statusBar()->showMessage("New row added", 1500);
}

void MainWindow::onEditClicked()
{
    const QModelIndex current = ui->tableEntries->currentIndex();
    if (!current.isValid()) {
        return;
    }

    QModelIndex editableIndex = current;
    if (current.column() == PasswordTableModel::IdColumn) {
        editableIndex = current.sibling(current.row(), PasswordTableModel::TitleColumn);
    }

    ui->tableEntries->edit(editableIndex);
}

void MainWindow::onDeleteClicked()
{
    if (!m_model || !m_proxy) {
        return;
    }

    const QModelIndex current = ui->tableEntries->currentIndex();
    if (!current.isValid()) {
        return;
    }

    const auto answer = QMessageBox::question(
        this,
        "Delete entry",
        "Delete selected record?"
        );

    if (answer != QMessageBox::Yes) {
        return;
    }

    const QModelIndex sourceIndex = m_proxy->mapToSource(current);
    QString error;

    if (!m_model->deleteRow(sourceIndex.row(), &error)) {
        showError("Delete error", error);
        return;
    }

    refreshCategoryFilter();
    applyFilters();
    statusBar()->showMessage("Row deleted", 1500);
}

void MainWindow::onSaveClicked()
{
    if (!m_model) {
        return;
    }

    QString error;
    if (!m_model->saveAll(&error)) {
        showError("Save error", error);
        return;
    }

    if (!m_model->load(&error)) {
        showError("Reload error", error);
        return;
    }

    refreshCategoryFilter();
    applyFilters();

    statusBar()->showMessage("Changes saved", 2000);
}

void MainWindow::onExitClicked()
{
    close();
}

void MainWindow::showError(const QString &title, const QString &message)
{
    QMessageBox::critical(this, title, message);
}
