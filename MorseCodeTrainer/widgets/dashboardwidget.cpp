#include "dashboardwidget.h"
#include "ui_dashboardwidget.h"
#include "database/databasemanager.h"
#include "services/settingsmanager.h"
#include <QPushButton>
#include <QFile>
#include <QTextStream>

static QStringList loadSymbolsForDashboard()
{
    QString language =
        SettingsManager::language();

    QFile file("../../data/symbols.txt");

    QStringList symbols;

    if (!file.open(
            QIODevice::ReadOnly |
            QIODevice::Text))
    {
        return symbols;
    }

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line =
            in.readLine().trimmed();

        if (line.isEmpty())
            continue;

        QStringList parts =
            line.split(';');

        if (parts.size() < 3)
            continue;

        QString group =
            parts[0].trimmed();

        QString symbol =
            parts[1].trimmed();

        if (language == "English")
        {
            if (group == "EN"
                || group == "NUM")
            {
                symbols.append(symbol);
            }
        }
        else
        {
            if (group == "UA"
                || group == "NUM")
            {
                symbols.append(symbol);
            }
        }
    }

    return symbols;
}

DashboardWidget::DashboardWidget(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DashboardWidget)
{
    ui->setupUi(this);

    for (QPushButton* button :
         findChildren<QPushButton*>())
    {
        button->setFocusPolicy(
            Qt::NoFocus);
    }

    updateProgress();

    connect(ui->startLearnButton,
            &QPushButton::clicked,
            this,
            &DashboardWidget::startLearnClicked);

    connect(ui->referenceButton,
            &QPushButton::clicked,
            this,
            &DashboardWidget::referenceClicked);

    connect(ui->statisticsButton,
            &QPushButton::clicked,
            this,
            &DashboardWidget::statisticsClicked);
}

void DashboardWidget::updateProgress()
{
    StatisticsData statistics =
        DatabaseManager::loadStatistics();

    QStringList symbols =
        loadSymbolsForDashboard();

    int learnedSymbols = 0;

    for (const QString& symbol :
         symbols)
    {
        int progress =
            statistics.symbolProgress.value(
                symbol.toUpper(),
                0);

        if (progress >= 3)
        {
            learnedSymbols++;
        }
    }

    int percent = 0;

    if (!symbols.isEmpty())
    {
        percent =
            learnedSymbols * 100
            / symbols.size();
    }

    ui->progressBar->setValue(
        percent);
}

DashboardWidget::~DashboardWidget()
{
    delete ui;
}
