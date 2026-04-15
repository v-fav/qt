#include "passwordtablemodel.h"
#include "passwordrepository.h"

#include <QDateTime>
#include <QSet>
#include <QSqlError>
#include <QDebug>

PasswordTableModel::PasswordTableModel(PasswordRepository *repository, QObject *parent)
    : QAbstractTableModel(parent),
    m_repository(repository)
{
}

int PasswordTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_items.size();
}

int PasswordTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return ColumnCount;
}

QVariant PasswordTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_items.size())
        return {};

    const PasswordRecord &item = m_items[index.row()];

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
        case IdColumn: return item.id;
        case TitleColumn: return item.title;
        case LoginColumn: return item.username;
        case PasswordColumn: return item.password;
        case WebsiteColumn: return item.website;
        case CategoryColumn: return item.category;
        case UpdatedAtColumn: return item.updatedAt;
        default: return {};
        }
    }

    return {};
}

QVariant PasswordTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return {};

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case IdColumn: return "ID";
        case TitleColumn: return "Title";
        case LoginColumn: return "Login";
        case PasswordColumn: return "Password";
        case WebsiteColumn: return "Website";
        case CategoryColumn: return "Category";
        case UpdatedAtColumn: return "UpdatedAt";
        default: return {};
        }
    }

    return section + 1;
}

Qt::ItemFlags PasswordTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags f = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    if (index.column() != IdColumn) {
        f |= Qt::ItemIsEditable;
    }

    return f;
}

bool PasswordTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole || !m_repository)
        return false;

    PasswordRecord updated = m_items[index.row()];

    switch (index.column()) {
    case TitleColumn:
        updated.title = value.toString();
        break;
    case LoginColumn:
        updated.username = value.toString();
        break;
    case PasswordColumn:
        updated.password = value.toString();
        break;
    case WebsiteColumn:
        updated.website = value.toString();
        break;
    case CategoryColumn:
        updated.category = value.toString();
        break;
    case UpdatedAtColumn:
        updated.updatedAt = value.toString();
        break;
    default:
        return false;
    }

    updated.updatedAt = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    QString error;
    if (!m_repository->update(updated, &error)) {
        qWarning() << "Update error:" << error;
        return false;
    }

    m_items[index.row()] = updated;

    emit dataChanged(this->index(index.row(), 0),
                     this->index(index.row(), ColumnCount - 1),
                     {Qt::DisplayRole, Qt::EditRole});

    return true;
}

void PasswordTableModel::setItems(const QVector<PasswordRecord> &items)
{
    beginResetModel();
    m_items = items;
    endResetModel();
}

bool PasswordTableModel::load(QString *error)
{
    if (!m_repository) {
        if (error) *error = "Repository is not initialized.";
        return false;
    }

    const QVector<PasswordRecord> items = m_repository->loadAll(error);
    if (error && !error->isEmpty())
        return false;

    setItems(items);
    return true;
}

bool PasswordTableModel::deleteRow(int row, QString *error)
{
    if (!m_repository || row < 0 || row >= m_items.size()) {
        if (error) *error = "Invalid row.";
        return false;
    }

    const int id = m_items[row].id;

    if (id > 0) {
        if (!m_repository->remove(id, error)) {
            return false;
        }
    }

    beginRemoveRows(QModelIndex(), row, row);
    m_items.removeAt(row);
    endRemoveRows();

    return true;
}

const PasswordRecord &PasswordTableModel::itemAt(int row) const
{
    return m_items[row];
}

QStringList PasswordTableModel::categories() const
{
    QSet<QString> unique;
    for (const auto &item : m_items) {
        if (!item.category.trimmed().isEmpty()) {
            unique.insert(item.category.trimmed());
        }
    }
    return QStringList(unique.begin(), unique.end());
}
