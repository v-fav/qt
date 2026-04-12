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
