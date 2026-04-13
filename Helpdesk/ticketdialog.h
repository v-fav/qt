#ifndef TICKETDIALOG_H
#define TICKETDIALOG_H
#include "ticket.h"

#include <QDialog>

namespace Ui {
class TicketDialog;
}

class TicketDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TicketDialog(QWidget *parent = nullptr);
    ~TicketDialog();
    void setTicket(const Ticket& t);
    Ticket getTicket() const;

private:
    Ui::TicketDialog *ui;
};

#endif // TICKETDIALOG_H
