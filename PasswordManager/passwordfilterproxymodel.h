#pragma once

#include <QSortFilterProxyModel>

class PasswordFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit PasswordFilterProxyModel(QObject *parent = nullptr);

    void setSearchText(const QString &text);
    void setCategoryFilter(const QString &category);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    QString m_searchText;
    QString m_category = "All";
};
