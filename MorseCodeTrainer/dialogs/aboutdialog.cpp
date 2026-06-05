#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include <QPushButton>

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    for (QPushButton* button :
         findChildren<QPushButton*>())
    {
        button->setFocusPolicy(
            Qt::NoFocus);
    }

    setWindowTitle("About");

    connect(ui->closeButton,
            &QPushButton::clicked,
            this,
            &QDialog::accept);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
