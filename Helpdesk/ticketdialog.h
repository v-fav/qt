#ifndef TICKETDIALOG_H
#define TICKETDIALOG_H

#include <QDialog>
#include "Ticket.h"

namespace Ui {
class TicketDialog;
}

class TicketDialog : public QDialog
{
    Q_OBJECT

public:
    enum class Mode {
        View,
        Edit,
        Create
    };

    explicit TicketDialog(QWidget *parent = nullptr);
    ~TicketDialog();

    void setMode(Mode mode);
    void loadTicket(const Ticket& ticket);
    Ticket collectTicket() const;

signals:
    void createRequested(const Ticket& ticket);
    void updateRequested(const Ticket& ticket);

private slots:
    void onFormChanged();
    void onEditClicked();
    void onCloseClicked();

private:
    void updateUiForMode();
    void updateButtonsState();
    bool isFormValid() const;

private:
    Ui::TicketDialog *ui;
    Mode mode_ = Mode::View;
};

#endif
