#pragma once

#include <QString>

struct CheckResult
{
    bool found = false;
    int count = 0;
    QString error;
};

CheckResult checkPasswordSync(const QString &password);
