#ifndef LESSONSELECTIONWIDGET_H
#define LESSONSELECTIONWIDGET_H
#include "models/testinfo.h"
#include <QDialog>

namespace Ui {
class LessonSelectionWidget;
}

class LessonSelectionWidget : public QDialog
{
    Q_OBJECT

public:
    explicit LessonSelectionWidget(QWidget *parent = nullptr);
    ~LessonSelectionWidget();
    TestInfo selectedTest() const;
    void reloadTests();

signals:
    void backRequested();
    void startLessonRequested();

private:
    Ui::LessonSelectionWidget *ui;

    QList<TestInfo> tests;

    int selectedTestIndex = -1;
};

#endif // LESSONSELECTIONWIDGET_H
