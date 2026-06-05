#include "databasemanager.h"

#include <QSettings>

StatisticsData DatabaseManager::loadStatistics()
{
    StatisticsData data;

    QSettings settings;

    int count =
        settings.value(
                    "completedTests/count",
                    0).toInt();

    for (int i = 0; i < count; i++)
    {
        TestResult result;

        result.testName =
            settings.value(
                        QString("completedTests/%1/name")
                            .arg(i)).toString();

        result.accuracy =
            settings.value(
                        QString("completedTests/%1/accuracy")
                            .arg(i)).toInt();

        data.completedTests.append(
            result);
    }

    QStringList keys =
        settings.value(
                    "symbols").toStringList();

    for (const QString& key : keys)
    {
        data.symbolProgress[key] =
            settings.value(
                        QString("symbolProgress/%1")
                            .arg(key)).toInt();
    }

    return data;
}

void DatabaseManager::saveStatistics(
    const StatisticsData& data)
{
    QSettings settings;

    settings.remove(
        "completedTests");

    settings.remove(
        "symbolProgress");

    settings.setValue(
        "completedTests/count",
        data.completedTests.size());

    for (int i = 0;
         i < data.completedTests.size();
         i++)
    {
        settings.setValue(
            QString("completedTests/%1/name")
                .arg(i),
            data.completedTests[i].testName);

        settings.setValue(
            QString("completedTests/%1/accuracy")
                .arg(i),
            data.completedTests[i].accuracy);
    }

    QStringList keys;

    for (auto it =
         data.symbolProgress.begin();
         it != data.symbolProgress.end();
         ++it)
    {
        keys.append(
            it.key());

        settings.setValue(
            QString("symbolProgress/%1")
                .arg(it.key()),
            it.value());
    }

    settings.setValue(
        "symbols",
        keys);

    settings.sync();
}

void DatabaseManager::resetStatistics()
{
    QSettings settings;

    settings.remove("completedTests");
    settings.remove("symbolProgress");

    settings.sync();
}
