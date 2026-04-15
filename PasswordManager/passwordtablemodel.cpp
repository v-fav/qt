#include "passwordtablemodel.h"
#include "passwordrepository.h"

#include <QDateTime>
#include <QSet>

PasswordTableModel::PasswordTableModel(PasswordRepository *repository, QObject *parent)
    : QAbstractTableModel(parent),
    m_repository(repository)
{
}

int PasswordTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return m_records.size();
}

int PasswordTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return ColumnCount;
}

QVariant PasswordTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_records.size()) {
        return {};
    }

    const PasswordRecord &r = m_records[index.row()];

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
        case IdColumn: return r.id;
        case TitleColumn: return r.title;
        case UsernameColumn: return r.username;
        case PasswordColumn: return r.password;
        case WebsiteColumn: return r.website;
        case CategoryColumn: return r.category;
        case UpdatedAtColumn: return r.updatedAt;
        default: return {};
        }
    }

    return {};
}

QVariant PasswordTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return {};
    }

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case IdColumn: return "ID";
        case TitleColumn: return "Title";
        case UsernameColumn: return "Username";
        case PasswordColumn: return "Password";
        case WebsiteColumn: return "Website";
        case CategoryColumn: return "Category";
        case UpdatedAtColumn: return "Updated At";
        default: return {};
        }
    }

    return section + 1;
}

Qt::ItemFlags PasswordTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    Qt::ItemFlags baseFlags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    if (index.column() != IdColumn) {
        baseFlags |= Qt::ItemIsEditable;
    }

    return baseFlags;
}

bool PasswordTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole) {
        return false;
    }

    PasswordRecord &r = m_records[index.row()];

    switch (index.column()) {
    case TitleColumn:
        r.title = value.toString();
        break;
    case UsernameColumn:
        r.username = value.toString();
        break;
    case PasswordColumn:
        r.password = value.toString();
        break;
    case WebsiteColumn:
        r.website = value.toString();
        break;
    case CategoryColumn:
        r.category = value.toString();
        break;
    case UpdatedAtColumn:
        r.updatedAt = value.toString();
        break;
    default:
        return false;
    }

    r.dirty = true;
    if (r.id == 0) {
        r.isNew = true;
    }
    touchRow(index.row());

    emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
    return true;
}

bool PasswordTableModel::load(QString *error)
{
    if (!m_repository) {
        if (error) *error = "Repository is not initialized.";
        return false;
    }

    beginResetModel();
    m_records = m_repository->loadAll(error);
    endResetModel();

    return error ? error->isEmpty() : true;
}

bool PasswordTableModel::saveAll(QString *error)
{
    if (!m_repository) {
        if (error) *error = "Repository is not initialized.";
        return false;
    }

    for (int i = 0; i < m_records.size(); ++i) {
        PasswordRecord &r = m_records[i];

        if (!r.dirty) {
            continue;
        }

        if (r.updatedAt.isEmpty()) {
            r.updatedAt = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        }

        QString localError;

        if (r.id == 0) {
            if (!m_repository->insert(r, &localError)) {
                if (error) *error = localError;
                return false;
            }
        } else {
            if (!m_repository->update(r, &localError)) {
                if (error) *error = localError;
                return false;
            }
        }

        r.dirty = false;
        r.isNew = false;
        emit dataChanged(index(i, 0), index(i, ColumnCount - 1), {Qt::DisplayRole, Qt::EditRole});
    }

    return true;
}

bool PasswordTableModel::addEmptyRow(const QString &defaultCategory)
{
    const int newRow = m_records.size();

    beginInsertRows(QModelIndex(), newRow, newRow);

    PasswordRecord record;
    record.category = defaultCategory;
    record.updatedAt = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    record.isNew = true;
    record.dirty = true;

    m_records.append(record);

    endInsertRows();
    return true;
}

bool PasswordTableModel::deleteRow(int row, QString *error)
{
    if (row < 0 || row >= m_records.size()) {
        if (error) *error = "Invalid row.";
        return false;
    }

    const int id = m_records[row].id;
    if (id > 0) {
        QString localError;
        if (!m_repository->remove(id, &localError)) {
            if (error) *error = localError;
            return false;
        }
    }

    beginRemoveRows(QModelIndex(), row, row);
    m_records.removeAt(row);
    endRemoveRows();

    return true;
}

QStringList PasswordTableModel::categories() const
{
    QSet<QString> unique;
    for (const auto &r : m_records) {
        if (!r.category.trimmed().isEmpty()) {
            unique.insert(r.category.trimmed());
        }
    }
    return QStringList(unique.begin(), unique.end());
}

void PasswordTableModel::touchRow(int row)
{
    if (row < 0 || row >= m_records.size()) {
        return;
    }

    m_records[row].updatedAt = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    const QModelIndex idx = index(row, UpdatedAtColumn);
    emit dataChanged(idx, idx, {Qt::DisplayRole, Qt::EditRole});
}
