#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QTextStream>
#include <QDebug>
#include <QList>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , lineIndex(0)
    , charIndex(0)
    , lastInputCorrect(true)
    , activeKeyButton(nullptr)
    , sessionTimer(new QTimer(this))
    , settings("TypingTrainerApp", "TypingTrainer")
    , totalTyped(0)
    , correctTyped(0)
    , wrongTyped(0)
    , sessionActive(false)
{
    ui->setupUi(this);

    ui->stackScreens->setCurrentWidget(ui->pageStart);
    this->setFocusPolicy(Qt::StrongFocus);

    typedPart = "";
    wrongChar = "";

    if (ui->comboSpeedMode->count() == 0) {
        ui->comboSpeedMode->addItem("CPM");
        ui->comboSpeedMode->addItem("WPM");
    }

    connect(sessionTimer, &QTimer::timeout, this, [this]() {
        updateStats();
    });

    scanLessons();
    loadSettings();

    if (ui->comboLesson->count() > 0) {
        QString path = ui->comboLesson->currentData().toString();
        loadLessonFromFile(path);
        updateLessonDescription();
        updateTrainingView();
        updateStats();
    }

    connect(ui->comboLesson, &QComboBox::currentIndexChanged, this, [this](int index) {
        if (index < 0) {
            return;
        }

        QString path = ui->comboLesson->currentData().toString();
        loadLessonFromFile(path);
        updateLessonDescription();
        updateTrainingView();
        saveSettings();
    });

    connect(ui->comboSpeedMode, &QComboBox::currentTextChanged, this, [this](const QString &) {
        updateStats();
        saveSettings();
    });

    connect(ui->btnStartTraining, &QPushButton::clicked, this, [this]() {
        if (ui->comboLesson->count() == 0) {
            return;
        }

        resetPosition();
        typedPart = "";
        wrongChar = "";
        lastInputCorrect = true;
        resetVirtualKeyboard();

        startSession();

        ui->stackScreens->setCurrentWidget(ui->pageTraining);
        updateTrainingView();
        updateStats();
        this->setFocus();
    });

    connect(ui->btnRestartTraining, &QPushButton::clicked, this, [this]() {
        resetPosition();
        typedPart = "";
        wrongChar = "";
        lastInputCorrect = true;
        resetVirtualKeyboard();

        startSession();

        ui->stackScreens->setCurrentWidget(ui->pageTraining);
        updateTrainingView();
        updateStats();
        this->setFocus();
    });

    connect(ui->btnReturnToMain, &QPushButton::clicked, this, [this]() {
        stopSession();
        resetVirtualKeyboard();
        ui->stackScreens->setCurrentWidget(ui->pageStart);
    });

    connect(ui->btnRandomLesson, &QPushButton::clicked, this, [this]() {
        chooseRandomLesson();
    });

    connect(ui->btnReloadLessons, &QPushButton::clicked, this, [this]() {
        QString oldPath = ui->comboLesson->currentData().toString();

        scanLessons();

        int restoreIndex = -1;
        for (int i = 0; i < ui->comboLesson->count(); i++) {
            if (ui->comboLesson->itemData(i).toString() == oldPath) {
                restoreIndex = i;
                break;
            }
        }

        if (restoreIndex >= 0) {
            ui->comboLesson->setCurrentIndex(restoreIndex);
        } else if (ui->comboLesson->count() > 0) {
            ui->comboLesson->setCurrentIndex(0);
        }

        if (ui->comboLesson->count() > 0) {
            QString path = ui->comboLesson->currentData().toString();
            loadLessonFromFile(path);
            updateLessonDescription();
            updateTrainingView();
        }

        saveSettings();
    });

    connect(ui->actionExit, &QAction::triggered, this, [this]() {
        saveSettings();
        close();
    });
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

void MainWindow::scanLessons()
{
    QString lessonsDirPath = "C:/qtt/pr10 TypingTrainer (final)/lessons";
    QDir dir(lessonsDirPath);

    ui->comboLesson->clear();

    if (!dir.exists()) {
        qDebug() << "Lessons directory does not exist:" << lessonsDirPath;
        ui->comboLesson->setEnabled(false);
        ui->btnStartTraining->setEnabled(false);
        ui->btnRandomLesson->setEnabled(false);
        ui->labelLessonDescription->setText("Lessons folder not found.");
        return;
    }

    QStringList files = dir.entryList(QStringList() << "*.txt", QDir::Files);

    if (files.isEmpty()) {
        qDebug() << "No lesson files found in:" << lessonsDirPath;
        ui->comboLesson->setEnabled(false);
        ui->btnStartTraining->setEnabled(false);
        ui->btnRandomLesson->setEnabled(false);
        ui->labelLessonDescription->setText("No lessons found.");
        return;
    }

    for (int i = 0; i < files.size(); i++) {
        QString fileName = files[i];
        QString fullPath = dir.filePath(fileName);

        QFileInfo fi(fullPath);
        QString title = fi.baseName();

        ui->comboLesson->addItem(title, fullPath);
    }

    ui->comboLesson->setEnabled(true);
    ui->btnStartTraining->setEnabled(true);
    ui->btnRandomLesson->setEnabled(true);
}

void MainWindow::loadLessonFromFile(const QString &path)
{
    if (path.isEmpty()) {
        currentLessonText = "";
        lessonLines.clear();
        lessonLines << "";
        resetPosition();
        typedPart = "";
        wrongChar = "";
        lastInputCorrect = true;
        return;
    }

    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file:" << path;
        qDebug() << file.errorString();

        QMessageBox::warning(this, "Error", "Cannot open lesson file:\n" + path);

        currentLessonText = "";
        lessonLines.clear();
        lessonLines << "";
        resetPosition();
        typedPart = "";
        wrongChar = "";
        lastInputCorrect = true;
        return;
    }

    QTextStream in(&file);
    QString text = in.readAll();
    file.close();

    loadLessonText(text);
}

void MainWindow::loadLessonText(const QString &text)
{
    currentLessonText = text;
    currentLessonText.replace("\r\n", "\n");
    currentLessonText.replace('\r', '\n');

    lessonLines = currentLessonText.split('\n');

    if (lessonLines.isEmpty()) {
        lessonLines << "";
    }

    resetPosition();
    typedPart = "";
    wrongChar = "";
    lastInputCorrect = true;
}

void MainWindow::resetPosition()
{
    lineIndex = 0;
    charIndex = 0;
}

void MainWindow::updateLessonDescription()
{
    if (ui->comboLesson->count() == 0) {
        ui->labelLessonDescription->setText("No lesson selected.");
        return;
    }

    QString path = ui->comboLesson->currentData().toString();
    QFileInfo fi(path);

    QString text = "File: " + fi.fileName() +
                   " | Size: " + QString::number(fi.size()) + " bytes";

    ui->labelLessonDescription->setText(text);
}

QString MainWindow::previousLine() const
{
    if (lineIndex <= 0 || lineIndex - 1 >= lessonLines.size()) {
        return "";
    }

    return lessonLines[lineIndex - 1];
}

QString MainWindow::currentLine() const
{
    if (lineIndex < 0 || lineIndex >= lessonLines.size()) {
        return "";
    }

    return lessonLines[lineIndex];
}

QString MainWindow::passedPart() const
{
    QString line = currentLine();

    if (charIndex <= 0) {
        return "";
    }

    if (charIndex >= line.size()) {
        return line;
    }

    return line.left(charIndex);
}

QString MainWindow::remainingPart() const
{
    QString line = currentLine();

    if (charIndex <= 0) {
        return line;
    }

    if (charIndex >= line.size()) {
        return "";
    }

    return line.mid(charIndex);
}

QString MainWindow::currentExpectedChar() const
{
    QString line = currentLine();

    if (charIndex < 0 || charIndex >= line.size()) {
        return "";
    }

    return line.mid(charIndex, 1);
}

void MainWindow::processTypedCharacter(const QString &text)
{
    if (!sessionActive) {
        return;
    }

    if (lessonLines.isEmpty()) {
        return;
    }

    if (lineIndex < 0 || lineIndex >= lessonLines.size()) {
        return;
    }

    QString line = currentLine();

    if (charIndex >= line.size()) {
        return;
    }

    QString expected = currentExpectedChar();

    totalTyped++;

    if (text == expected) {
        typedPart += text;
        wrongChar = "";
        lastInputCorrect = true;
        charIndex++;
        correctTyped++;
    } else {
        wrongChar = text;
        lastInputCorrect = false;
        wrongTyped++;
    }

    updateStats();

    if (lineIndex == lessonLines.size() - 1 && charIndex >= currentLine().size()) {
        finishSession();
    }
}

void MainWindow::moveBack()
{
    if (!sessionActive) {
        return;
    }

    wrongChar = "";
    lastInputCorrect = true;

    if (charIndex > 0) {
        charIndex--;

        if (!typedPart.isEmpty()) {
            typedPart.chop(1);
        }
        return;
    }

    if (lineIndex > 0) {
        lineIndex--;
        QString line = currentLine();
        charIndex = line.size();
        typedPart = line;
    }
}

void MainWindow::moveNextLine()
{
    if (!sessionActive) {
        return;
    }

    QString line = currentLine();

    if (charIndex == line.size()) {
        if (lineIndex + 1 < lessonLines.size()) {
            lineIndex++;
            charIndex = 0;
            typedPart = "";
            wrongChar = "";
            lastInputCorrect = true;
        } else {
            finishSession();
        }
    }
}

void MainWindow::updateTrainingView()
{
    QString prev = previousLine();
    QString line = currentLine();

    QString pre = line.left(charIndex);
    QString cur = line.mid(charIndex, 1);
    QString post = line.mid(charIndex + 1);

    QString html;
    html += "<html><body style='font-family: Courier New; font-size: 14pt;'>";

    if (!prev.isEmpty()) {
        html += "<p style='color: gray; margin: 4px 0;'>";
        html += prev.toHtmlEscaped();
        html += "</p>";
    } else {
        html += "<p style='color: gray; margin: 4px 0;'>&nbsp;</p>";
    }

    html += "<p style='margin: 4px 0;'>";

    if (!pre.isEmpty()) {
        html += "<span style='background-color:#d8f5c0;'>";
        html += pre.toHtmlEscaped();
        html += "</span>";
    }

    if (!cur.isEmpty()) {
        if (lastInputCorrect) {
            html += "<span style='background-color:#ffe08a;'>";
            html += cur.toHtmlEscaped();
            html += "</span>";
        } else {
            html += "<span style='background-color:#ffb3b3; color:black;'>";
            html += cur.toHtmlEscaped();
            html += "</span>";
        }
    }

    if (!post.isEmpty()) {
        html += post.toHtmlEscaped();
    }

    html += "</p>";

    if (!wrongChar.isEmpty()) {
        html += "<p style='color:#cc3333; margin: 4px 0;'>Wrong: ";
        html += wrongChar.toHtmlEscaped();
        html += "</p>";
    }

    html += "</body></html>";

    ui->textDisplay->setHtml(html);
}

void MainWindow::chooseRandomLesson()
{
    int count = ui->comboLesson->count();

    if (count <= 0) {
        return;
    }

    int randomIndex = QRandomGenerator::global()->bounded(count);
    ui->comboLesson->setCurrentIndex(randomIndex);
}

void MainWindow::resetVirtualKeyboard()
{
    QList<QPushButton*> buttons = ui->keyboardWidget->findChildren<QPushButton*>();

    for (QPushButton *btn : buttons) {
        btn->setStyleSheet("");
    }

    activeKeyButton = nullptr;
}

void MainWindow::highlightVirtualKey(QKeyEvent *event)
{
    resetVirtualKeyboard();

    QString keyName;

    if (event->key() >= Qt::Key_A && event->key() <= Qt::Key_Z) {
        if (!event->text().isEmpty()) {
            keyName = "key" + event->text().toUpper();
        }
    }
    else if (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9) {
        if (!event->text().isEmpty()) {
            keyName = "key" + event->text();
        }
    }
    else if (event->key() == Qt::Key_Space) keyName = "keySpace";
    else if (event->key() == Qt::Key_Backspace) keyName = "keyBackspace";
    else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) keyName = "keyEnter";
    else if (event->key() == Qt::Key_Tab) keyName = "keyTab";
    else if (event->key() == Qt::Key_Minus) keyName = "keyMinus";
    else if (event->key() == Qt::Key_Equal) keyName = "keyEqual";
    else if (event->key() == Qt::Key_Comma) keyName = "keyComma";
    else if (event->key() == Qt::Key_Period) keyName = "keyPeriod";
    else if (event->key() == Qt::Key_Slash) keyName = "keySlash";
    else if (event->key() == Qt::Key_Semicolon) keyName = "keySemicolon";
    else if (event->key() == Qt::Key_Apostrophe) keyName = "keyApostrophe";
    else if (event->key() == Qt::Key_BracketLeft) keyName = "keyBracketLeft";
    else if (event->key() == Qt::Key_BracketRight) keyName = "keyBracketRight";

    if (keyName.isEmpty()) {
        return;
    }

    QPushButton *btn = ui->keyboardWidget->findChild<QPushButton*>(keyName);

    if (btn) {
        btn->setStyleSheet("background-color: #a8d8ff;");
        activeKeyButton = btn;
    }
}

void MainWindow::startSession()
{
    resetSessionStats();
    elapsedTimer.start();
    sessionTimer->start(1000);
    sessionActive = true;
}

void MainWindow::stopSession()
{
    sessionTimer->stop();
    sessionActive = false;
}

void MainWindow::resetSessionStats()
{
    totalTyped = 0;
    correctTyped = 0;
    wrongTyped = 0;

    ui->labelTimeValue->setText("00:00");
    ui->labelSpeedValue->setText("0");
    ui->labelAccuracyValue->setText("0%");
    ui->progressAccuracy->setValue(0);
}

QString MainWindow::formattedTime(qint64 ms) const
{
    int totalSeconds = static_cast<int>(ms / 1000);
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    QString mm = QString::number(minutes).rightJustified(2, '0');
    QString ss = QString::number(seconds).rightJustified(2, '0');

    return mm + ":" + ss;
}

double MainWindow::speedValue() const
{
    if (!sessionActive && elapsedTimer.elapsed() <= 0) {
        return 0.0;
    }

    double minutes = elapsedTimer.elapsed() / 60000.0;

    if (minutes <= 0.0) {
        return 0.0;
    }

    QString mode = ui->comboSpeedMode->currentText();

    if (mode == "WPM") {
        return (correctTyped / 5.0) / minutes;
    }

    return correctTyped / minutes;
}

double MainWindow::accuracyValue() const
{
    if (totalTyped <= 0) {
        return 0.0;
    }

    return (correctTyped * 100.0) / totalTyped;
}

void MainWindow::updateStats()
{
    qint64 ms = elapsedTimer.isValid() ? elapsedTimer.elapsed() : 0;

    ui->labelTimeValue->setText(formattedTime(ms));

    double speed = speedValue();
    double acc = accuracyValue();

    QString speedMode = ui->comboSpeedMode->currentText();

    ui->labelSpeedValue->setText(QString::number(speed, 'f', 0) + " " + speedMode);
    ui->labelAccuracyValue->setText(QString::number(acc, 'f', 0) + "%");
    ui->progressAccuracy->setValue(static_cast<int>(acc));
}

void MainWindow::updateResultsPage()
{
    qint64 ms = elapsedTimer.isValid() ? elapsedTimer.elapsed() : 0;
    double speed = speedValue();
    double acc = accuracyValue();

    ui->labelResultTimeValue->setText(formattedTime(ms));
    ui->labelResultSpeedValue->setText(QString::number(speed, 'f', 0) + " " + ui->comboSpeedMode->currentText());
    ui->labelResultAccuracyValue->setText(QString::number(acc, 'f', 0) + "%");
}

void MainWindow::finishSession()
{
    stopSession();
    updateStats();
    updateResultsPage();
    ui->stackScreens->setCurrentWidget(ui->pageResults);
}

void MainWindow::loadSettings()
{
    QString savedSpeedMode = settings.value("ui/speedMode", "CPM").toString();
    int speedIndex = ui->comboSpeedMode->findText(savedSpeedMode);
    if (speedIndex >= 0) {
        ui->comboSpeedMode->setCurrentIndex(speedIndex);
    }

    QString savedLessonPath = settings.value("ui/lastLessonPath", "").toString();
    if (!savedLessonPath.isEmpty()) {
        for (int i = 0; i < ui->comboLesson->count(); i++) {
            if (ui->comboLesson->itemData(i).toString() == savedLessonPath) {
                ui->comboLesson->setCurrentIndex(i);
                break;
            }
        }
    }
}

void MainWindow::saveSettings()
{
    settings.setValue("ui/speedMode", ui->comboSpeedMode->currentText());
    settings.setValue("ui/lastLessonPath", ui->comboLesson->currentData().toString());
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (ui->stackScreens->currentWidget() != ui->pageTraining) {
        QMainWindow::keyPressEvent(event);
        return;
    }

    if (!sessionActive) {
        QMainWindow::keyPressEvent(event);
        return;
    }

    highlightVirtualKey(event);

    if (event->key() == Qt::Key_Backspace) {
        moveBack();
        updateTrainingView();
        updateStats();
        return;
    }

    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        moveNextLine();
        updateTrainingView();
        updateStats();
        return;
    }

    if (event->text().isEmpty()) {
        return;
    }

    processTypedCharacter(event->text());
    updateTrainingView();
    updateStats();
}
