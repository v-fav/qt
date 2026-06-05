#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "widgets/dashboardwidget.h"
#include "widgets/lessonselectionwidget.h"
#include "widgets/trainingwidget.h"
#include "widgets/referencewidget.h"
#include "widgets/statisticswidget.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    DashboardWidget* dashboardWidget;
    LessonSelectionWidget* lessonSelectionWidget;
    TrainingWidget* trainingWidget;
    ReferenceWidget* referenceWidget;
    StatisticsWidget* statisticsWidget;

    void setupPages();
    void setupConnections();
};
#endif // MAINWINDOW_H
