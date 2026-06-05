#include "statisticswidget.h"
#include "ui_statisticswidget.h"

#include "services/testmanager.h"
#include "database/databasemanager.h"
#include "services/settingsmanager.h"
#include "dialogs/confirmresetdialog.h"

#include <QFile>
#include <QTableWidgetItem>
#include <QTextStream>
#include <QMap>
#include <QHeaderView>
#include <QFileDialog>
#include <QPushButton>
#include <QFont>
#include <QStandardPaths>

static QStringList loadSymbolsForCurrentLanguage()
{
    QString language = SettingsManager::language();

    QFile file("../../data/symbols.txt");
    QStringList symbols;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return symbols;
    }

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        if (line.isEmpty())
            continue;

        QStringList parts = line.split(';');

        if (parts.size() < 3)
            continue;

        QString group = parts[0].trimmed();
        QString symbol = parts[1].trimmed();

        if (language.compare("English", Qt::CaseInsensitive) == 0)
        {
            if (group == "EN" || group == "NUM")
            {
                symbols.append(symbol);
            }
        }
        else
        {
            if (group == "UA" || group == "NUM")
            {
                symbols.append(symbol);
            }
        }
    }

    return symbols;
}

StatisticsWidget::StatisticsWidget(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StatisticsWidget)
{
    ui->setupUi(this);

    for (QPushButton* button :
         findChildren<QPushButton*>())
    {
        button->setFocusPolicy(
            Qt::NoFocus);
    }

    ui->label_2->setContentsMargins(0, 12, 0, 0);
    ui->label_3->setContentsMargins(0, 12, 0, 0);

    ui->totalSessionsLabel->setContentsMargins(0, 0, 0, 0);
    ui->averageAccuracyLabel->setContentsMargins(0, 0, 0, 0);

    QFont statisticsFont;
    statisticsFont.setPointSize(18);

    ui->totalSessionsLabel->setFont(statisticsFont);
    ui->averageAccuracyLabel->setFont(statisticsFont);

    ui->totalSessionsLabel->setAlignment(Qt::AlignCenter);
    ui->averageAccuracyLabel->setAlignment(Qt::AlignCenter);

    ui->accuracyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->weakSymbolsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->accuracyTable->horizontalHeader()->setStretchLastSection(true);
    ui->weakSymbolsTable->horizontalHeader()->setStretchLastSection(true);

    ui->accuracyTable->verticalHeader()->setVisible(false);
    ui->weakSymbolsTable->verticalHeader()->setVisible(false);

    ui->accuracyTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->weakSymbolsTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    connect(ui->exportButton,
            &QPushButton::clicked,
            this,
            &StatisticsWidget::exportStatistics);

    connect(ui->backButton,
            &QPushButton::clicked,
            this,
            [this]()
            {
                emit backRequested();
            });

    connect(ui->resetButton,
            &QPushButton::clicked,
            this,
            [this]()
            {
                ConfirmResetDialog dialog(this);

                connect(&dialog,
                        &ConfirmResetDialog::confirmed,
                        this,
                        [this]()
                        {
                            DatabaseManager::resetStatistics();
                            reloadStatistics();
                        });

                dialog.exec();
            });

    reloadStatistics();
}

void StatisticsWidget::reloadStatistics()
{
    ui->accuracyTable->clearContents();
    ui->accuracyTable->setRowCount(0);

    ui->weakSymbolsTable->clearContents();
    ui->weakSymbolsTable->setRowCount(0);

    StatisticsData statistics = DatabaseManager::loadStatistics();

    QMap<QString, int> uniqueResults;

    TestManager manager;

    QList<TestInfo> allTests =
        manager.loadTests(
            "../../data/tests");

    QString currentLanguage =
        SettingsManager::language();

    for (const TestResult& result :
         statistics.completedTests)
    {
        bool showTest = false;

        for (const TestInfo& test :
             allTests)
        {
            if (test.title != result.testName)
            {
                continue;
            }

            if (test.language == "Any")
            {
                showTest = true;
            }
            else if (
                currentLanguage == "English"
                && test.language == "English")
            {
                showTest = true;
            }
            else if (
                currentLanguage == "Ukrainian"
                && test.language == "Ukrainian")
            {
                showTest = true;
            }

            break;
        }

        if (showTest)
        {
            uniqueResults[result.testName] =
                result.accuracy;
        }
    }

    ui->accuracyTable->setRowCount(uniqueResults.size());

    int row = 0;
    int sum = 0;

    for (auto it = uniqueResults.begin(); it != uniqueResults.end(); ++it)
    {
        QTableWidgetItem* testItem = new QTableWidgetItem(it.key());
        testItem->setTextAlignment(Qt::AlignCenter);
        ui->accuracyTable->setItem(row, 0, testItem);

        QTableWidgetItem* accuracyItem = new QTableWidgetItem(QString("%1%").arg(it.value()));
        accuracyItem->setTextAlignment(Qt::AlignCenter);
        ui->accuracyTable->setItem(row, 1, accuracyItem);

        sum += it.value();
        row++;
    }

    double average = 0;
    if (!uniqueResults.isEmpty())
    {
        average = static_cast<double>(sum) / uniqueResults.size();
    }

    ui->averageAccuracyLabel->setText(
        QString("%1%").arg(average, 0, 'f', 0));

    int totalTests = 0;

    for (const TestInfo& test :
         allTests)
    {
        if (test.language == "Any")
        {
            totalTests++;
        }
        else if (
            currentLanguage == "English"
            && test.language == "English")
        {
            totalTests++;
        }
        else if (
            currentLanguage == "Ukrainian"
            && test.language == "Ukrainian")
        {
            totalTests++;
        }
    }

    ui->totalSessionsLabel->setText(
        QString("%1/%2")
            .arg(
                uniqueResults.size())
            .arg(
                totalTests));

    QStringList symbols = loadSymbolsForCurrentLanguage();

    int weakSymbolsCount = 0;

    for (const QString& symbol : symbols)
    {
        int progress = statistics.symbolProgress.value(symbol.toUpper(), 0);

        if (progress >= 3)
        {
            continue;
        }

        weakSymbolsCount++;

        int currentRow = ui->weakSymbolsTable->rowCount();
        ui->weakSymbolsTable->insertRow(currentRow);

        QTableWidgetItem* symbolItem = new QTableWidgetItem(symbol);
        symbolItem->setTextAlignment(Qt::AlignCenter);
        ui->weakSymbolsTable->setItem(currentRow, 0, symbolItem);

        QTableWidgetItem* progressItem = new QTableWidgetItem(QString("%1/3").arg(progress));
        progressItem->setTextAlignment(Qt::AlignCenter);
        ui->weakSymbolsTable->setItem(currentRow, 1, progressItem);
    }

    if (weakSymbolsCount == 0)
    {
        ui->weakSymbolsTable->setRowCount(1);
        ui->weakSymbolsTable->setSpan(0, 0, 1, 2);

        QTableWidgetItem* messageItem = new QTableWidgetItem(
            "Congratulations! You have learned all Morse code symbols.");

        messageItem->setTextAlignment(Qt::AlignCenter);
        ui->weakSymbolsTable->setItem(0, 0, messageItem);
    }
}

void StatisticsWidget::exportStatistics()
{
    QString downloadsFolder =
        QStandardPaths::writableLocation(
            QStandardPaths::DownloadLocation);

    QString defaultFile =
        downloadsFolder +
        "/Morse Code Trainer Statistics.txt";

    QString fileName =
        QFileDialog::getSaveFileName(
            this,
            "Export statistics",
            defaultFile,
            "Text files (*.txt)");

    if (fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream out(&file);

    StatisticsData statistics = DatabaseManager::loadStatistics();

    out << "MORSE CODE TRAINER STATISTICS\n";
    out << "============================\n\n";

    out << "Completed tests:\n\n";

    QMap<QString, int> uniqueResults;

    for (const TestResult& result : statistics.completedTests)
    {
        uniqueResults[result.testName] = result.accuracy;
    }

    double sum = 0;

    for (auto it = uniqueResults.begin(); it != uniqueResults.end(); ++it)
    {
        out << it.key() << " - " << it.value() << "%\n";
        sum += it.value();
    }

    double average = 0;
    if (!uniqueResults.isEmpty())
    {
        average = sum / uniqueResults.size();
    }

    out << "\nAverage accuracy: "
        << QString::number(average, 'f', 0)
        << "%\n\n";

    out << "Weak symbols:\n\n";

    QStringList symbols = loadSymbolsForCurrentLanguage();

    bool hasWeakSymbols = false;

    for (const QString& symbol : symbols)
    {
        int progress = statistics.symbolProgress.value(symbol.toUpper(), 0);

        if (progress >= 3)
        {
            continue;
        }

        hasWeakSymbols = true;
        out << symbol << " - " << progress << "/3\n";
    }

    if (!hasWeakSymbols)
    {
        out << "Congratulations! You have learned all Morse code symbols.\n";
    }

    file.close();
}

StatisticsWidget::~StatisticsWidget()
{
    delete ui;
}
