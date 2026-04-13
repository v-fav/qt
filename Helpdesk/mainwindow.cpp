#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QItemSelectionModel>
#include "ticketdialog.h"
#include <QDialog>

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

void MainWindow::updateActions() {
    bool hasSelection = ui->ticketsTableView->currentIndex().isValid();

    ui->actionViewTicket->setEnabled(hasSelection);
    ui->actionEditTicket->setEnabled(hasSelection);
    ui->actionDeleteTicket->setEnabled(hasSelection);
}

void MainWindow::on_actionDeleteTicket_triggered() {
    QModelIndex index = ui->ticketsTableView->currentIndex();
    if (!index.isValid()) return;

    model->removeTicket(index.row());
}

void MainWindow::on_actionNewTicket_triggered() {
    Ticket t;
    t.id = model->rowCount() + 1;
    t.title = "New Ticket";
    t.priority = "Low";
    t.status = "Open";
    t.createdAt = "2024-04-25";

    model->addTicket(t);
}

void MainWindow::on_actionEditTicket_triggered() {
    QModelIndex index = ui->ticketsTableView->currentIndex();
    if (!index.isValid()) return;

    int row = index.row();
    Ticket t = model->getTicket(row);

    TicketDialog dialog(this);
    dialog.setTicket(t);

    if (dialog.exec() == QDialog::Accepted) {
        model->updateTicket(row, dialog.getTicket());
    }
}
