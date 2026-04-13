#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QItemSelectionModel>
#include <QMessageBox>
#include "ticketdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , repository("tickets.csv")
{
    ui->setupUi(this);

    model = new TicketTableModel(this);
    ui->ticketsTableView->setModel(model);
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
}

MainWindow::~MainWindow() {
    delete ui;
}

int MainWindow::currentRow() const {
    auto rows = ui->ticketsTableView->selectionModel()->selectedRows();
    if (rows.isEmpty()) return -1;
    return rows.first().row();
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

    connect(dialog, &TicketDialog::createRequested, this,
            [this](const Ticket& t){

                Ticket newTicket = t;
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
