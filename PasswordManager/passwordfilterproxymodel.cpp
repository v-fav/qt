#include "passwordfilterproxymodel.h"
#include "passwordtablemodel.h"

PasswordFilterProxyModel::PasswordFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setDynamicSortFilter(true);
}

void PasswordFilterProxyModel::setSearchText(const QString &text)
{
    m_searchText = text.trimmed();
    invalidateFilter();
}

void PasswordFilterProxyModel::setCategoryFilter(const QString &category)
{
    m_category = category;
    invalidateFilter();
}

bool PasswordFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const auto *source = sourceModel();
    if (!source)
        return true;

    const QModelIndex titleIndex = source->index(source_row, PasswordTableModel::TitleColumn, source_parent);
    const QModelIndex categoryIndex = source->index(source_row, PasswordTableModel::CategoryColumn, source_parent);

    const QString title = source->data(titleIndex).toString();
    const QString category = source->data(categoryIndex).toString();

    const bool textOk = m_searchText.isEmpty()
                        || title.contains(m_searchText, Qt::CaseInsensitive);

    const bool categoryOk = (m_category == "All" || m_category.isEmpty())
                            || category == m_category;

    return textOk && categoryOk;
}
