#include "passwordrepository.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

PasswordRepository::PasswordRepository(const QSqlDatabase &db)
    : m_db(db)
{
}

QVector<PasswordRecord> PasswordRepository::loadAll(QString *error) const
{
    QVector<PasswordRecord> items;

    QSqlQuery query(m_db);
    if (!query.exec(
            "SELECT id, title, username, password, website, category, updated_at "
            "FROM password_items "
            "ORDER BY id DESC"))
    {
        if (error) *error = query.lastError().text();
        return items;
    }

    while (query.next()) {
        PasswordRecord item;
        item.id = query.value(0).toInt();
        item.title = query.value(1).toString();
        item.username = query.value(2).toString();
        item.password = query.value(3).toString();
        item.website = query.value(4).toString();
        item.category = query.value(5).toString();
        item.updatedAt = query.value(6).toString();
        item.isNew = false;
        item.dirty = false;
        items.append(item);
    }

    return items;
}

bool PasswordRepository::insert(PasswordRecord &record, QString *error) const
{
    QSqlQuery query(m_db);
    query.prepare(
        "INSERT INTO password_items "
        "(title, username, password, website, category, updated_at) "
        "VALUES "
        "(:title, :username, :password, :website, :category, :updated_at)"
        );

    query.bindValue(":title", record.title);
    query.bindValue(":username", record.username);
    query.bindValue(":password", record.password);
    query.bindValue(":website", record.website);
    query.bindValue(":category", record.category);
    query.bindValue(":updated_at", record.updatedAt);

    if (!query.exec()) {
        if (error) *error = query.lastError().text();
        return false;
    }

    record.id = query.lastInsertId().toInt();
    return true;
}

bool PasswordRepository::update(const PasswordRecord &record, QString *error) const
{
    QSqlQuery query(m_db);
    query.prepare(
        "UPDATE password_items SET "
        "title = :title, "
        "username = :username, "
        "password = :password, "
        "website = :website, "
        "category = :category, "
        "updated_at = :updated_at "
        "WHERE id = :id"
        );

    query.bindValue(":title", record.title);
    query.bindValue(":username", record.username);
    query.bindValue(":password", record.password);
    query.bindValue(":website", record.website);
    query.bindValue(":category", record.category);
    query.bindValue(":updated_at", record.updatedAt);
    query.bindValue(":id", record.id);

    if (!query.exec()) {
        if (error) *error = query.lastError().text();
        return false;
    }

    return true;
}

bool PasswordRepository::remove(int id, QString *error) const
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM password_items WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        if (error) *error = query.lastError().text();
        return false;
    }

    return true;
}
