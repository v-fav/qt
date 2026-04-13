#include "TicketFilterProxyModel.h"
#include "tickettablemodel.h"

TicketFilterProxyModel::TicketFilterProxyModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
}

void TicketFilterProxyModel::setStatusFilter(const QString& status)
{
    status_ = status;
    invalidateFilter();
}

void TicketFilterProxyModel::setPriorityFilter(const QString& priority)
{
    priority_ = priority;
    invalidateFilter();
}

void TicketFilterProxyModel::setSearchText(const QString& text)
{
    text_ = text;
    invalidateFilter();
}

bool TicketFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex& parent) const
{
    auto model = sourceModel();

    QString title = model->index(sourceRow, TicketTableModel::TitleCol).data().toString();
    QString status = model->index(sourceRow, TicketTableModel::StatusCol).data().toString();
    QString priority = model->index(sourceRow, TicketTableModel::PriorityCol).data().toString();

    bool textOk = text_.isEmpty() || title.contains(text_, Qt::CaseInsensitive);
    bool statusOk = (status_ == "All" || status_.isEmpty()) || status == status_;
    bool priorityOk = (priority_ == "All" || priority_.isEmpty()) || priority == priority_;

    return textOk && statusOk && priorityOk;
}
