#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QStringList>

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

    QString currentLessonText;
    QStringList lessonLines;

    int lineIndex;
    int charIndex;

    void scanLessons();
    void loadLessonFromFile(const QString &path);
    void loadLessonText(const QString &text);
    void resetPosition();
    void updateLessonDescription();
    void updateTrainingView();
    void stepForward();
    void chooseRandomLesson();

    QString previousLine() const;
    QString currentLine() const;
    QString passedPart() const;
    QString remainingPart() const;
};

#endif // MAINWINDOW_H
