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
    auto *model = sourceModel();
    if (!model) {
        return true;
    }

    const QModelIndex categoryIndex = model->index(source_row, PasswordTableModel::CategoryColumn, source_parent);
    const QString categoryValue = model->data(categoryIndex, Qt::DisplayRole).toString();

    if (!m_category.isEmpty() && m_category != "All" && categoryValue != m_category) {
        return false;
    }

    if (m_searchText.isEmpty()) {
        return true;
    }

    QString combined;
    combined += model->data(model->index(source_row, PasswordTableModel::TitleColumn, source_parent)).toString();
    combined += ' ';
    combined += model->data(model->index(source_row, PasswordTableModel::UsernameColumn, source_parent)).toString();
    combined += ' ';
    combined += model->data(model->index(source_row, PasswordTableModel::WebsiteColumn, source_parent)).toString();
    combined += ' ';
    combined += categoryValue;

    return combined.contains(m_searchText, Qt::CaseInsensitive);
}
