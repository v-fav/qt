#ifndef TICKETDIALOG_H
#define TICKETDIALOG_H

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

private:
    Ui::TicketDialog *ui;
};

#endif // TICKETDIALOG_H
