#ifndef TRAININGWIDGET_H
#define TRAININGWIDGET_H
#include "models/testinfo.h"
#include <QDialog>
#include "services/morseencoder.h"
#include <QKeyEvent>
#include <QElapsedTimer>

namespace Ui {
class TrainingWidget;
}

class TrainingWidget : public QDialog
{
    Q_OBJECT

public:
    explicit TrainingWidget(QWidget *parent = nullptr);
    ~TrainingWidget();
    void setTest(
        const TestInfo& test);

signals:
    void exitRequested();
    void dashboardRequested();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(
        QKeyEvent* event) override;
private:
    Ui::TrainingWidget *ui;
    TestInfo currentTest;

    int currentQuestionIndex = 0;

    void updateQuestion();

    MorseEncoder encoder;

    QString currentInput;

    void updateTranslation();

    int correctAnswers = 0;
    int wrongAnswers = 0;

    void finishTest();

    QElapsedTimer spaceTimer;

    bool spacePressed = false;


};

#endif // TRAININGWIDGET_H
