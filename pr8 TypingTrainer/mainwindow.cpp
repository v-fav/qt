#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QTextStream>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , lineIndex(0)
    , charIndex(0)
{
    ui->setupUi(this);

    scanLessons();

    if (ui->comboLesson->count() > 0) {
        QString path = ui->comboLesson->currentData().toString();
        loadLessonFromFile(path);
        updateLessonDescription();
        updateTrainingView();
    }

    connect(ui->comboLesson, &QComboBox::currentIndexChanged, this, [this](int index) {
        if (index < 0) {
            return;
        }

        QString path = ui->comboLesson->currentData().toString();
        loadLessonFromFile(path);
        updateLessonDescription();
        updateTrainingView();
    });

    connect(ui->btnStartTraining, &QPushButton::clicked, this, [this]() {
        if (ui->comboLesson->count() == 0) {
            return;
        }

        ui->stackScreens->setCurrentWidget(ui->pageTraining);
        updateTrainingView();
    });

    connect(ui->btnRestartTraining, &QPushButton::clicked, this, [this]() {
        resetPosition();
        ui->stackScreens->setCurrentWidget(ui->pageTraining);
        updateTrainingView();
    });

    connect(ui->btnReturnToMain, &QPushButton::clicked, this, [this]() {
        ui->stackScreens->setCurrentWidget(ui->pageStart);
    });

    connect(ui->btnNextChar, &QPushButton::clicked, this, [this]() {
        stepForward();
    });

    connect(ui->btnRandomLesson, &QPushButton::clicked, this, [this]() {
        chooseRandomLesson();
    });

    connect(ui->btnReloadLessons, &QPushButton::clicked, this, [this]() {
        scanLessons();

        if (ui->comboLesson->count() > 0) {
            QString path = ui->comboLesson->currentData().toString();
            loadLessonFromFile(path);
            updateLessonDescription();
            updateTrainingView();
        }
    });

    connect(ui->actionExit, &QAction::triggered, this, [this]() {
        close();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::scanLessons()
{
    QString lessonsDirPath = "C:/qtt/pr8 TypingTrainer/lessons";
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

void MainWindow::stepForward()
{
    if (lessonLines.isEmpty()) {
        return;
    }

    if (lineIndex < 0 || lineIndex >= lessonLines.size()) {
        return;
    }

    QString line = lessonLines[lineIndex];

    if (charIndex < line.size()) {
        charIndex++;
    } else {
        if (lineIndex + 1 < lessonLines.size()) {
            lineIndex++;
            charIndex = 0;
        }
    }

    updateTrainingView();
}

void MainWindow::updateTrainingView()
{
    QString prev = previousLine();
    QString currentPassed = passedPart();
    QString currentRest = remainingPart();

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

    if (!currentPassed.isEmpty()) {
        html += "<span style='background-color: #f3d37a;'>";
        html += currentPassed.toHtmlEscaped();
        html += "</span>";
    }

    if (!currentRest.isEmpty()) {
        QString currentChar = currentRest.left(1);
        QString afterChar = currentRest.mid(1);

        html += "<span style='background-color: #f2a3a3;'>";
        html += currentChar.toHtmlEscaped();
        html += "</span>";

        html += afterChar.toHtmlEscaped();
    }

    html += "</p>";
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
