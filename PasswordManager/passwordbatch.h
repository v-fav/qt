#pragma once

#include <QVector>
#include "passwordrecord.h"
#include "passwordcheckutils.h"
#include <functional>

struct BatchItem
{
    int row;
    QString password;
};

struct BatchResult
{
    int row;
    CheckResult result;
};

QVector<BatchResult> checkAllPasswords(
    const QVector<BatchItem> &items,
    std::function<void(int,int)> progress = nullptr
    );
