#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QStringList>
#include <QKeyEvent>
#include <QPushButton>
#include <QTimer>
#include <QElapsedTimer>
#include <QSettings>

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

    QTimer *sessionTimer;
    QElapsedTimer elapsedTimer;
    QSettings settings;

    int totalTyped;
    int correctTyped;
    int wrongTyped;
    bool sessionActive;

    void scanLessons();
    void loadLessonFromFile(const QString &path);
    void loadLessonText(const QString &text);
    void resetPosition();

    void updateLessonDescription();
    void updateTrainingView();
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

    void startSession();
    void stopSession();
    void resetSessionStats();
    void updateStats();
    void finishSession();

    QString formattedTime(qint64 ms) const;
    double speedValue() const;
    double accuracyValue() const;
    void updateResultsPage();

    void loadSettings();
    void saveSettings();
};

#endif // MAINWINDOW_H
