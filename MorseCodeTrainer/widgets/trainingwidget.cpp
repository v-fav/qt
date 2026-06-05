#include "trainingwidget.h"
#include "ui_trainingwidget.h"
#include <QMessageBox>
#include "services/settingsmanager.h"
#include "services/morseencoder.h"
#include "dialogs/sessionsummarydialog.h"
#include "database/databasemanager.h"
#include "models/statisticsdata.h"

#include <QPushButton>

TrainingWidget::TrainingWidget(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TrainingWidget)
{
    ui->setupUi(this);

    QFont questionFont;

    questionFont.setPointSize(14);

    ui->questionNumberLabel->setFont(
        questionFont);

    QFont taskFont;

    taskFont.setPointSize(30);
    taskFont.setBold(true);

    ui->taskLabel->setFont(taskFont);

    ui->taskLabel->setAlignment(
        Qt::AlignCenter);

    ui->dotButton->setFocusPolicy(Qt::NoFocus);
    ui->dashButton->setFocusPolicy(Qt::NoFocus);
    ui->clearButton->setFocusPolicy(Qt::NoFocus);
    ui->submitButton->setFocusPolicy(Qt::NoFocus);
    ui->skipButton->setFocusPolicy(Qt::NoFocus);
    ui->exitButton->setFocusPolicy(Qt::NoFocus);

    ui->morseInputField->setFocusPolicy(
        Qt::NoFocus);

    ui->progressBar->setValue(0);
    ui->questionNumberLabel->setText("");
    ui->taskLabel->setText("");

    encoder.loadSymbols("../../data/symbols.txt");

    ui->morseInputField->setText("");
    ui->translationLabel->setText("");
    ui->feedbackLabel->setText("");

    connect(ui->skipButton,
            &QPushButton::clicked,
            this,
            [this]()
            {
                wrongAnswers++;

                ui->feedbackLabel->setText("Skipped");

                currentInput.clear();
                ui->morseInputField->clear();
                ui->translationLabel->clear();

                if (currentQuestionIndex + 1 < currentTest.questions.size())
                {
                    currentQuestionIndex++;
                    updateQuestion();
                }
                else
                {
                    finishTest();
                }
            });

    connect(ui->submitButton,
            &QPushButton::clicked,
            this,
            [this]()
            {
                if (currentInput.trimmed().isEmpty())
                {
                    QMessageBox::warning(
                        this,
                        "Warning",
                        "Please enter a Morse code answer or skip this task.");

                    return;
                }

                QString answer =
                    encoder.decodeForLanguage(
                        currentInput,
                        SettingsManager::language());

                QString expected =
                    currentTest.questions[currentQuestionIndex];

                if (answer.compare(expected, Qt::CaseInsensitive) == 0)
                {
                    ui->feedbackLabel->setText("Correct");
                    correctAnswers++;
                    StatisticsData statistics = DatabaseManager::loadStatistics();

                    const QString symbolKey = expected.trimmed().toUpper();

                    statistics.symbolProgress[symbolKey] =
                        statistics.symbolProgress.value(symbolKey, 0) + 1;

                    DatabaseManager::saveStatistics(statistics);
                }
                else
                {
                    ui->feedbackLabel->setText("Incorrect");
                    wrongAnswers++;
                }

                currentInput.clear();
                ui->morseInputField->clear();
                ui->translationLabel->clear();

                if (currentQuestionIndex + 1 < currentTest.questions.size())
                {
                    currentQuestionIndex++;
                    updateQuestion();
                }
                else
                {
                    finishTest();
                }
            });

    connect(ui->dotButton,
            &QPushButton::clicked,
            this,
            [this]()
            {
                currentInput += ".";
                ui->morseInputField->setText(currentInput);
                updateTranslation();
            });

    connect(ui->dashButton,
            &QPushButton::clicked,
            this,
            [this]()
            {
                currentInput += "-";
                ui->morseInputField->setText(currentInput);
                updateTranslation();
            });

    connect(ui->clearButton,
            &QPushButton::clicked,
            this,
            [this]()
            {
                if (currentInput.isEmpty())
                    return;

                currentInput.chop(1);
                ui->morseInputField->setText(currentInput);
                updateTranslation();
            });

    connect(ui->exitButton,
            &QPushButton::clicked,
            this,
            &TrainingWidget::exitRequested);

    setFocusPolicy(Qt::StrongFocus);
    setFocus();
}

TrainingWidget::~TrainingWidget()
{
    delete ui;
}

void TrainingWidget::setTest(const TestInfo& test)
{
    currentTest = test;
    currentQuestionIndex = 0;
    correctAnswers = 0;
    wrongAnswers = 0;

    updateQuestion();
    setFocus();
}

void TrainingWidget::updateQuestion()
{
    if (currentTest.questions.isEmpty())
    {
        return;
    }

    ui->questionNumberLabel->setText(
        QString("Task %1/%2")
            .arg(currentQuestionIndex + 1)
            .arg(currentTest.questions.size()));

    ui->taskLabel->setText(
        currentTest.questions[currentQuestionIndex]);

    ui->taskLabel->setAlignment(
        Qt::AlignCenter);

    int progress =
        ((currentQuestionIndex + 1) * 100) / currentTest.questions.size();

    ui->progressBar->setValue(progress);
}

void TrainingWidget::updateTranslation()
{
    QString symbol =
        encoder.decodeForLanguage(
            currentInput,
            SettingsManager::language());

    ui->translationLabel->setText(symbol);
}

void TrainingWidget::finishTest()
{
    int total = correctAnswers + wrongAnswers;
    int accuracy = 0;

    if (total > 0)
    {
        accuracy = correctAnswers * 100 / total;
    }

    StatisticsData statistics = DatabaseManager::loadStatistics();

    bool found = false;

    for (auto& test : statistics.completedTests)
    {
        if (test.testName == currentTest.title)
        {
            if (accuracy > test.accuracy)
            {
                test.accuracy = accuracy;
            }

            found = true;
            break;
        }
    }

    if (!found)
    {
        TestResult result;
        result.testName = currentTest.title;
        result.accuracy = accuracy;
        statistics.completedTests.append(result);
    }

    DatabaseManager::saveStatistics(statistics);

    emit dashboardRequested();

    SessionSummaryDialog dlg(this);

    connect(&dlg,
            &SessionSummaryDialog::backToDashboardRequested,
            this,
            &TrainingWidget::dashboardRequested);

    dlg.setResults(correctAnswers, wrongAnswers);
    dlg.exec();
}

void TrainingWidget::keyPressEvent(
    QKeyEvent* event)
{
    if (SettingsManager::
        spaceInputEnabled()
        &&
        event->key() == Qt::Key_Space)
    {
        spaceTimer.start();

        spacePressed = true;

        return;
    }

    if (!SettingsManager::
        keyboardInputEnabled())
    {
        QDialog::keyPressEvent(event);
        return;
    }

    if (event->key() == Qt::Key_Period)
    {
        currentInput += ".";

        ui->morseInputField->setText(
            currentInput);

        updateTranslation();

        return;
    }

    if (event->key() == Qt::Key_Minus)
    {
        currentInput += "-";

        ui->morseInputField->setText(
            currentInput);

        updateTranslation();

        return;
    }

    if (event->key() == Qt::Key_Backspace)
    {
        if (!currentInput.isEmpty())
        {
            currentInput.chop(1);

            ui->morseInputField->setText(
                currentInput);

            updateTranslation();
        }

        return;
    }

    if (event->key() == Qt::Key_Return
        || event->key() == Qt::Key_Enter)
    {
        ui->submitButton->click();

        return;
    }

    QDialog::keyPressEvent(event);
}

void TrainingWidget::keyReleaseEvent(
    QKeyEvent* event)
{
    if (!SettingsManager::
        spaceInputEnabled())
    {
        QDialog::keyReleaseEvent(event);
        return;
    }

    if (event->key() == Qt::Key_Space
        && spacePressed)
    {
        spacePressed = false;

        qint64 duration =
            spaceTimer.elapsed();

        if (duration < 300)
        {
            currentInput += ".";
        }
        else
        {
            currentInput += "-";
        }

        ui->morseInputField->setText(
            currentInput);

        updateTranslation();
    }

    QDialog::keyReleaseEvent(event);
}
