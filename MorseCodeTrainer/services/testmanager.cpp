#include "testmanager.h"

#include <QDir>
#include <QFile>
#include <QTextStream>

QList<TestInfo> TestManager::loadTests(
    const QString& folderPath)
{
    QList<TestInfo> tests;

    QDir dir(folderPath);

    QStringList files =
        dir.entryList(
            QStringList() << "*.txt",
            QDir::Files);

    for (const QString& file : files)
    {
        tests.append(
            loadTest(
                dir.filePath(file)));
    }

    return tests;
}

TestInfo TestManager::loadTest(
    const QString& filePath)
{
    TestInfo test;

    QFile file(filePath);

    if (!file.open(
            QIODevice::ReadOnly |
            QIODevice::Text))
    {
        return test;
    }

    test.fileName = filePath;

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line =
            in.readLine().trimmed();

        if (line.isEmpty())
            continue;

        if (line.startsWith("Name="))
        {
            test.title =
                line.mid(5);

            continue;
        }

        if (line.startsWith("Difficulty="))
        {
            test.difficulty =
                line.mid(11);

            continue;
        }

        if (line.startsWith("Language="))
        {
            test.language =
                line.mid(9);

            continue;
        }

        test.questions.append(line);
    }

    return test;
}
