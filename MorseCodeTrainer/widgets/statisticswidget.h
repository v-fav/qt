#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H

#include <QDialog>

namespace Ui {
class StatisticsWidget;
}

class StatisticsWidget : public QDialog
{
    Q_OBJECT

public:
    explicit StatisticsWidget(QWidget *parent = nullptr);
    ~StatisticsWidget();
    void reloadStatistics();

signals:
    void backRequested();

private:
    Ui::StatisticsWidget *ui;

private slots:
    void exportStatistics();
};

#endif // STATISTICSWIDGET_H
