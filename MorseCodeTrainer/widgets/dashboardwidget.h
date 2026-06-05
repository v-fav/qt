#ifndef DASHBOARDWIDGET_H
#define DASHBOARDWIDGET_H

#include <QDialog>

namespace Ui {
class DashboardWidget;
}

class DashboardWidget : public QDialog
{
    Q_OBJECT

public:
    explicit DashboardWidget(QWidget *parent = nullptr);
    ~DashboardWidget();
    void updateProgress();

signals:
    void startLearnClicked();
    void referenceClicked();
    void statisticsClicked();
    void settingsClicked();

private:
    Ui::DashboardWidget *ui;
};

#endif // DASHBOARDWIDGET_H
