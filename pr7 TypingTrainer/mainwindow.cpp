#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , lineIndex(0)
    , charIndex(0)
{
    ui->setupUi(this);

    setupLessons();

    QString lessonName = ui->comboLesson->currentText();
    loadLessonText(lessons.value(lessonName));
    updateLessonDescription(lessonName);
    updateTrainingView();

    connect(ui->comboLesson, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        loadLessonText(lessons.value(text));
        updateLessonDescription(text);
        updateTrainingView();
    });

    connect(ui->btnStartTraining, &QPushButton::clicked, this, [this]() {
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

    connect(ui->actionExit, &QAction::triggered, this, [this]() {
        close();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupLessons()
{
    lessons["Starter Text"] =
        "jfj j j ff ffjjjj fjjjj ff jjjj jjjj\n"
        "jfj j jff ffjjjj Fjn ffj\n"
        "jjf fjf jjj fff jj.";

    lessons["Numbers and Symbols"] =
        "123 456 7890\n"
        "1 2 3, 4 5 6.\n"
        "7-8-9 / 0 = done!";

    lessons["Common Words Part 1"] =
        "the cat is here\n"
        "we type short words\n"
        "practice makes progress";

    lessons["Common Words Part 2"] =
        "small steps are important\n"
        "typing needs rhythm and focus\n"
        "keep your hands relaxed";

    lessons["Python Code Sample"] =
        "print('Hello, world!')\n"
        "for i in range(3):\n"
        "    print(i)";

    lessons["Lorem Ipsum"] =
        "Lorem ipsum dolor sit amet,\n"
        "consectetur adipiscing elit.\n"
        "Sed do eiusmod tempor.";
}

void MainWindow::updateLessonDescription(const QString &lessonName)
{
    if (lessonName == "Starter Text") {
        ui->labelLessonDescription->setText("Starter lesson for basic finger warm-up.");
    }
    else if (lessonName == "Numbers and Symbols") {
        ui->labelLessonDescription->setText("Digits and punctuation practice.");
    }
    else if (lessonName == "Common Words Part 1") {
        ui->labelLessonDescription->setText("Short common words for steady typing.");
    }
    else if (lessonName == "Common Words Part 2") {
        ui->labelLessonDescription->setText("Longer phrases and rhythm practice.");
    }
    else if (lessonName == "Python Code Sample") {
        ui->labelLessonDescription->setText("Typing code-like text with symbols.");
    }
    else if (lessonName == "Lorem Ipsum") {
        ui->labelLessonDescription->setText("Generic text for free typing practice.");
    }
    else {
        ui->labelLessonDescription->setText("");
    }
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
    }
    else {
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
    }
    else {
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
