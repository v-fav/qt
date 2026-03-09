#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QStringList>
#include <QKeyEvent>
#include <QPushButton>

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

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui;

    QString currentLessonText;
    QStringList lessonLines;

    int lineIndex;
    int charIndex;

    QString typedPart;
    bool lastInputCorrect;
    QString wrongChar;

    QPushButton *activeKeyButton;

    void scanLessons();
    void loadLessonFromFile(const QString &path);
    void loadLessonText(const QString &text);
    void resetPosition();
    void updateLessonDescription();
    void updateTrainingView();
    void stepForward();
    void chooseRandomLesson();

    void processTypedCharacter(const QString &text);
    void moveBack();
    void moveNextLine();
    void highlightVirtualKey(QKeyEvent *event);
    void resetVirtualKeyboard();
    QString currentExpectedChar() const;

    QString previousLine() const;
    QString currentLine() const;
    QString passedPart() const;
    QString remainingPart() const;
};

#endif // MAINWINDOW_H
