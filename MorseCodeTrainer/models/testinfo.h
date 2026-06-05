#ifndef TESTINFO_H
#define TESTINFO_H

#include <QString>
#include <QStringList>

struct TestInfo
{
    QString fileName;

    QString title;

    QString difficulty;

    QString language;

    QStringList questions;
};

#endif // TESTINFO_H
