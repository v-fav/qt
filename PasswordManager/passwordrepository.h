#pragma once

#include <QSqlDatabase>
#include <QVector>
#include "passwordrecord.h"

class PasswordRepository
{
public:
    explicit PasswordRepository(const QSqlDatabase &db);

    QVector<PasswordRecord> loadAll(QString *error = nullptr) const;
    bool insert(PasswordRecord &record, QString *error = nullptr) const;
    bool update(const PasswordRecord &record, QString *error = nullptr) const;
    bool remove(int id, QString *error = nullptr) const;

private:
    QSqlDatabase m_db;
};
