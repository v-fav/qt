#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QItemSelectionModel>
#include <QMessageBox>
#include <QHeaderView>
#include "ticketdialog.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , repository("tickets.csv")
{
    ui->setupUi(this);

    ui->statusFilterComboBox->addItems({
        "All",
        "Open",
        "In Progress",
        "Closed"
    });

    ui->priorityFilterComboBox->addItems({
        "All",
        "Low",
        "Medium",
        "High"
    });

    model = new TicketTableModel(this);
    proxyModel = new TicketFilterProxyModel(this);
    proxyModel->setSourceModel(model);

    ui->ticketsTableView->setModel(proxyModel);
    ui->ticketsTableView->setSortingEnabled(true);
    ui->ticketsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->ticketsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->ticketsTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    auto tickets = repository.loadAll();
    model->setTickets(tickets);

    connect(ui->ticketsTableView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &MainWindow::updateActions);

    updateActions();

    connect(ui->actionExit, &QAction::triggered,
            this, &QWidget::close);

    connect(ui->actionAbout, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, "About Helpdesk",
                           "Helpdesk Application\n\n"
                           "A simple ticket management system.\n"
                           "Supports creating, editing, filtering,\n"
                           "searching and sorting tickets.\n\n"
                           "Made with Qt.");
    });

    connect(proxyModel, &QAbstractItemModel::modelReset,
            this, &MainWindow::updateEmptyState);

    connect(proxyModel, &QAbstractItemModel::rowsInserted,
            this, &MainWindow::updateEmptyState);

    connect(proxyModel, &QAbstractItemModel::rowsRemoved,
            this, &MainWindow::updateEmptyState);

    connect(ui->statusFilterComboBox, &QComboBox::currentTextChanged,
            this, &MainWindow::updateEmptyState);

    connect(ui->priorityFilterComboBox, &QComboBox::currentTextChanged,
            this, &MainWindow::updateEmptyState);

    connect(ui->searchLineEdit, &QLineEdit::textChanged,
            this, &MainWindow::updateEmptyState);

    connect(ui->statusFilterComboBox, &QComboBox::currentTextChanged,
            proxyModel, &TicketFilterProxyModel::setStatusFilter);

    connect(ui->priorityFilterComboBox, &QComboBox::currentTextChanged,
            proxyModel, &TicketFilterProxyModel::setPriorityFilter);

    connect(ui->searchLineEdit, &QLineEdit::textChanged,
            proxyModel, &TicketFilterProxyModel::setSearchText);

    connect(ui->ticketsTableView, &QTableView::doubleClicked,
            this, [this](const QModelIndex& proxyIndex){

                QModelIndex sourceIndex = proxyModel->mapToSource(proxyIndex);
                int row = sourceIndex.row();

                if (row < 0) return;

                auto* dialog = new TicketDialog(this);
                dialog->setAttribute(Qt::WA_DeleteOnClose);

                dialog->loadTicket(model->getTicket(row));
                dialog->setMode(TicketDialog::Mode::View);

                dialog->show();
            });
    updateEmptyState();
}

MainWindow::~MainWindow() {
    delete ui;
}

int MainWindow::currentRow() const {
    QModelIndex proxyIndex = ui->ticketsTableView->currentIndex();
    if (!proxyIndex.isValid()) return -1;

    QModelIndex sourceIndex = proxyModel->mapToSource(proxyIndex);
    return sourceIndex.row();
}

void MainWindow::updateActions() {
    bool hasSelection = ui->ticketsTableView->selectionModel()->hasSelection();

    ui->actionViewTicket->setEnabled(hasSelection);
    ui->actionEditTicket->setEnabled(hasSelection);
    ui->actionDeleteTicket->setEnabled(hasSelection);
}

void MainWindow::on_actionNewTicket_triggered() {
    auto* dialog = new TicketDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    dialog->setMode(TicketDialog::Mode::Create);

    int nextId = repository.getNextId(model->getAllTickets());
    Ticket temp;
    temp.id = nextId;

    temp.status = "Open";
    temp.priority = "Low";

    temp.createdAt = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");

    dialog->loadTicket(temp);

    connect(dialog, &TicketDialog::createRequested, this,
            [this](const Ticket& t){

                Ticket newTicket = t;
                newTicket.createdAt = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
                newTicket.id = repository.getNextId(model->getAllTickets());

                model->addTicket(newTicket);
                repository.saveAll(model->getAllTickets());
            });

    dialog->show();
}

void MainWindow::on_actionViewTicket_triggered() {
    int row = currentRow();
    if (row < 0) return;

    auto* dialog = new TicketDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    dialog->loadTicket(model->getTicket(row));
    dialog->setMode(TicketDialog::Mode::View);

    dialog->show();
}

void MainWindow::on_actionEditTicket_triggered() {
    int row = currentRow();
    if (row < 0) return;

    auto* dialog = new TicketDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    dialog->loadTicket(model->getTicket(row));
    dialog->setMode(TicketDialog::Mode::Edit);

    connect(dialog, &TicketDialog::updateRequested, this,
            [this, row](const Ticket& t){
                model->updateTicket(row, t);
                repository.saveAll(model->getAllTickets());
            });

    dialog->show();
}

void MainWindow::on_actionDeleteTicket_triggered() {
    int row = currentRow();
    if (row < 0) return;

    auto result = QMessageBox::question(
        this,
        "Delete Ticket",
        "Are you sure you want to delete this ticket?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (result == QMessageBox::Yes) {
        model->removeTicket(row);
        repository.saveAll(model->getAllTickets());
    }
}

void MainWindow::on_clearFiltersButton_clicked()
{
    ui->statusFilterComboBox->setCurrentText("All");
    ui->priorityFilterComboBox->setCurrentText("All");
    ui->searchLineEdit->clear();
}

void MainWindow::updateEmptyState()
{
    bool empty = proxyModel->rowCount() == 0;

    ui->emptyLabel->setVisible(empty);
    ui->ticketsTableView->setVisible(!empty);
}
