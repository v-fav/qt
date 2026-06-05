#ifndef SESSIONSUMMARYDIALOG_H
#define SESSIONSUMMARYDIALOG_H

#include <QDialog>

namespace Ui {
class SessionSummaryDialog;
}

class SessionSummaryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SessionSummaryDialog(QWidget *parent = nullptr);
    ~SessionSummaryDialog();
    void setResults(
        int correct,
        int wrong);

signals:
    void backToDashboardRequested();

private:
    Ui::SessionSummaryDialog *ui;
};

#endif // SESSIONSUMMARYDIALOG_H
