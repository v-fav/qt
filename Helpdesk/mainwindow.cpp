#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QItemSelectionModel>
#include <QMessageBox>
#include "ticketdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new TicketTableModel(this);
    ui->ticketsTableView->setModel(model);

    ui->ticketsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->ticketsTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    model->addTicket({1, "VPN issue", "High", "Open", "2024-04-24"});
    model->addTicket({2, "Email bug", "Low", "Closed", "2024-04-23"});
    model->addTicket({3, "Crash app", "High", "Open", "2024-04-22"});
    model->addTicket({4, "UI glitch", "Medium", "In Progress", "2024-04-21"});
    model->addTicket({5, "Login fail", "High", "Open", "2024-04-20"});

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
                model->addTicket(t);
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
    }
}
