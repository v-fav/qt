#ifndef TESTMANAGER_H
#define TESTMANAGER_H

#include <QList>

#include "models/testinfo.h"

class TestManager
{
public:
    QList<TestInfo> loadTests(
        const QString& folderPath);

    TestInfo loadTest(
        const QString& filePath);
};

#endif
