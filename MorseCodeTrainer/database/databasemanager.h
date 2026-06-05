#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "models/statisticsdata.h"

class DatabaseManager
{
public:
    static StatisticsData loadStatistics();

    static void saveStatistics(
        const StatisticsData& data);

    static void resetStatistics();
};

#endif
