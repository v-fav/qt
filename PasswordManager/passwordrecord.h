#pragma once

#include <QString>

struct PasswordRecord
{
    int id = 0;
    QString title;
    QString username;
    QString password;
    QString website;
    QString category;
    QString updatedAt;
    QString checkStatus;
    bool isNew = false;
    bool dirty = false;
};
