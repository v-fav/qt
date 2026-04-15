#include "databasemanager.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

DatabaseManager::DatabaseManager() = default;

DatabaseManager::~DatabaseManager()
{
    if (m_db.isValid() && m_db.isOpen()) {
        m_db.close();
    }
}

bool DatabaseManager::open(const QString &filePath)
{
    if (QSqlDatabase::contains(m_connectionName)) {
        m_db = QSqlDatabase::database(m_connectionName);
    } else {
        m_db = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);
    }

    if (m_db.isOpen()) {
        m_db.close();
    }

    m_db.setDatabaseName(filePath);

    if (!m_db.open()) {
        m_lastError = m_db.lastError().text();
        qCritical() << "Database open error:" << m_lastError;
        return false;
    }

    return true;
}

bool DatabaseManager::initializeSchema()
{
    if (!m_db.isOpen()) {
        m_lastError = "Database is not open.";
        return false;
    }

    QSqlQuery query(m_db);

    const QString sql = R"(
        CREATE TABLE IF NOT EXISTS password_items (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL DEFAULT '',
            username TEXT,
            password TEXT,
            website TEXT,
            category TEXT,
            updated_at TEXT NOT NULL DEFAULT ''
        )
    )";

    if (!query.exec(sql)) {
        m_lastError = query.lastError().text();
        qCritical() << "Schema init error:" << m_lastError;
        return false;
    }

    return true;
}

QSqlDatabase DatabaseManager::database() const
{
    return m_db;
}

QString DatabaseManager::lastError() const
{
    return m_lastError;
}
