#pragma once

#include <QSqlDatabase>
#include <QString>

class DatabaseManager
{
public:
    DatabaseManager();
    ~DatabaseManager();

    bool open(const QString &filePath);
    bool initializeSchema();
    QSqlDatabase database() const;
    QString lastError() const;

private:
    QSqlDatabase m_db;
    QString m_lastError;
    const QString m_connectionName = "password_manager_connection";
};
