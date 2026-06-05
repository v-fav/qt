#ifndef STATISTICSDATA_H
#define STATISTICSDATA_H

#include <QString>
#include <QMap>
#include <QList>

struct TestResult
{
    QString testName;
    int accuracy;
};

struct StatisticsData
{
    QList<TestResult> completedTests;

    QMap<QString, int> symbolProgress;
};

#endif
