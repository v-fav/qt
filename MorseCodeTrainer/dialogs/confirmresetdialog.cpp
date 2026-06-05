#include "confirmresetdialog.h"
#include "ui_confirmresetdialog.h"
#include <QPushButton>

ConfirmResetDialog::ConfirmResetDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConfirmResetDialog)
{
    ui->setupUi(this);

    for (QPushButton* button :
         findChildren<QPushButton*>())
    {
        button->setFocusPolicy(
            Qt::NoFocus);
    }

    connect(ui->yesButton,
            &QPushButton::clicked,
            this,
            [this]()
            {
                emit confirmed();
                accept();
            });

    connect(ui->noButton,
            &QPushButton::clicked,
            this,
            &QDialog::reject);
}

ConfirmResetDialog::~ConfirmResetDialog()
{
    delete ui;
}
