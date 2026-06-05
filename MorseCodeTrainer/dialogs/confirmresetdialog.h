#ifndef CONFIRMRESETDIALOG_H
#define CONFIRMRESETDIALOG_H

#include <QDialog>

namespace Ui {
class ConfirmResetDialog;
}

class ConfirmResetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmResetDialog(QWidget *parent = nullptr);
    ~ConfirmResetDialog();

signals:
    void confirmed();

private:
    Ui::ConfirmResetDialog *ui;
};

#endif // CONFIRMRESETDIALOG_H
