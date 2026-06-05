#include "lessonselectionwidget.h"
#include "ui_lessonselectionwidget.h"

#include "services/testmanager.h"
#include "services/settingsmanager.h"

#include <QHeaderView>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QAbstractItemView>

LessonSelectionWidget::LessonSelectionWidget(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LessonSelectionWidget)
{
    ui->setupUi(this);

    for (QPushButton* button :
         findChildren<QPushButton*>())
    {
        button->setFocusPolicy(
            Qt::NoFocus);
    }

    ui->testTable->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);
    ui->testTable->verticalHeader()
        ->setVisible(false);
    ui->testTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->testTable->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(ui->testTable,
            &QTableWidget::cellClicked,
            this,
            [this](int row, int)
            {
                if (row < 0 || row >= tests.size())
                {
                    return;
                }

                selectedTestIndex = row;

                const TestInfo& test = tests[row];

                ui->lessonPreviewTitle->setText(
                    "Test name: " + test.title);

                ui->taskCountLabel->setText(
                    "Tasks: " + QString::number(test.questions.size()));

                ui->difficultyLabel->setText(
                    "Difficulty: " + test.difficulty);

                ui->languageLabel->setText(
                    "Language: " + test.language);

                ui->startLessonButton->setEnabled(true);
            });

    connect(ui->backButton,
            &QPushButton::clicked,
            this,
            &LessonSelectionWidget::backRequested);

    connect(ui->startLessonButton,
            &QPushButton::clicked,
            this,
            &LessonSelectionWidget::startLessonRequested);

    reloadTests();
}

LessonSelectionWidget::~LessonSelectionWidget()
{
    delete ui;
}

void LessonSelectionWidget::reloadTests()
{
    selectedTestIndex = -1;
    tests.clear();

    ui->testTable->clearContents();
    ui->testTable->setRowCount(0);
    ui->testTable->clearSelection();

    ui->lessonPreviewTitle->clear();
    ui->taskCountLabel->clear();
    ui->difficultyLabel->clear();
    ui->languageLabel->clear();

    ui->startLessonButton->setEnabled(false);

    TestManager manager;
    QList<TestInfo> allTests = manager.loadTests("../../data/tests");

    QString currentLanguage = SettingsManager::language();

    for (const TestInfo& test : allTests)
    {
        if (test.language == "Any")
        {
            tests.append(test);
        }
        else if (currentLanguage == "English" && test.language == "English")
        {
            tests.append(test);
        }
        else if (currentLanguage == "Ukrainian" && test.language == "Ukrainian")
        {
            tests.append(test);
        }
    }

    ui->testTable->setRowCount(tests.size());

    for (int row = 0; row < tests.size(); ++row)
    {
        const TestInfo& test = tests[row];

        ui->testTable->setItem(row, 0, new QTableWidgetItem(test.title));
        ui->testTable->setItem(row, 1, new QTableWidgetItem(test.difficulty));
        ui->testTable->setItem(row, 2, new QTableWidgetItem(test.language));
        ui->testTable->setItem(
            row,
            3,
            new QTableWidgetItem(QString::number(test.questions.size())));
    }
}

TestInfo LessonSelectionWidget::selectedTest() const
{
    if (selectedTestIndex < 0 || selectedTestIndex >= tests.size())
    {
        return TestInfo();
    }

    return tests[selectedTestIndex];
}
