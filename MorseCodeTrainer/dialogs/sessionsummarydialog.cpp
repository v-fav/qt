#include "sessionsummarydialog.h"
#include "ui_sessionsummarydialog.h"
#include <QString>
#include <QPushButton>

SessionSummaryDialog::SessionSummaryDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SessionSummaryDialog)
{
    ui->setupUi(this);

    for (QPushButton* button :
         findChildren<QPushButton*>())
    {
        button->setFocusPolicy(
            Qt::NoFocus);
    }

    connect(ui->backButton,
            &QPushButton::clicked,
            this,
            [this]()
            {
                emit backToDashboardRequested();

                accept();
            });
}

SessionSummaryDialog::~SessionSummaryDialog()
{
    delete ui;
}

void SessionSummaryDialog::setResults(
    int correct,
    int wrong)
{
    int total =
        correct + wrong;

    int accuracy = 0;

    if (total > 0)
    {
        accuracy =
            correct * 100 / total;
    }

    ui->correctLabel->setText(
        QString::number(correct));

    ui->wrongLabel->setText(
        QString::number(wrong));

    ui->accuracyLabel->setText(
        QString("%1%")
            .arg(accuracy));

    ui->progressBar->setValue(
        accuracy);
}
