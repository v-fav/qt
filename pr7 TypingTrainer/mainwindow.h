#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QStringList>
#include <QMap>

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

    QMap<QString, QString> lessons;
    QString currentLessonText;
    QStringList lessonLines;

    int lineIndex;
    int charIndex;

    void setupLessons();
    void updateLessonDescription(const QString &lessonName);
    void loadLessonText(const QString &text);
    void resetPosition();
    void updateTrainingView();
    void stepForward();

    QString previousLine() const;
    QString currentLine() const;
    QString passedPart() const;
    QString remainingPart() const;
};

#endif // MAINWINDOW_H
