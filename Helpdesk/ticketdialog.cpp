#include "ticketdialog.h"
#include "ui_ticketdialog.h"

TicketDialog::TicketDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TicketDialog)
{
    ui->setupUi(this);
}

TicketDialog::~TicketDialog()
{
    delete ui;
}

void TicketDialog::setTicket(const Ticket& t) {
    ui->idValueLabel->setText(QString::number(t.id));
    ui->titleValueLabel->setText(t.title);
    ui->priorityComboBox->setCurrentText(t.priority);
    ui->statusComboBox->setCurrentText(t.status);
    ui->createdValueLabel->setText(t.createdAt);
}

Ticket TicketDialog::getTicket() const {
    Ticket t;
    t.id = ui->idValueLabel->text().toInt();
    t.title = ui->titleValueLabel->text();
    t.priority = ui->priorityComboBox->currentText();
    t.status = ui->statusComboBox->currentText();
    t.createdAt = ui->createdValueLabel->text();
    return t;
}
