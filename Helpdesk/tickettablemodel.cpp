#include "tickettablemodel.h"

TicketTableModel::TicketTableModel(QObject* parent)
    : QAbstractTableModel(parent) {}

int TicketTableModel::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : tickets_.size();
}

int TicketTableModel::columnCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : ColCount;
}

QVariant TicketTableModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) return {};

    const Ticket& t = tickets_.at(index.row());

    switch (index.column()) {
    case IdCol: return t.id;
    case TitleCol: return t.title;
    case PriorityCol: return t.priority;
    case StatusCol: return t.status;
    case CreatedCol: return t.createdAt;
    default: return {};
    }
}

QVariant TicketTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};

    switch (section) {
    case IdCol: return "ID";
    case TitleCol: return "Title";
    case PriorityCol: return "Priority";
    case StatusCol: return "Status";
    case CreatedCol: return "Created At";
    default: return {};
    }
}

void TicketTableModel::addTicket(const Ticket& ticket) {
    int row = tickets_.size();
    beginInsertRows(QModelIndex(), row, row);
    tickets_.push_back(ticket);
    endInsertRows();
}

void TicketTableModel::updateTicket(int row, const Ticket& ticket) {
    if (row < 0 || row >= tickets_.size()) return;

    tickets_[row] = ticket;

    QModelIndex left = index(row, 0);
    QModelIndex right = index(row, ColCount - 1);

    emit dataChanged(left, right);
}

void TicketTableModel::removeTicket(int row) {
    if (row < 0 || row >= tickets_.size()) return;

    beginRemoveRows(QModelIndex(), row, row);
    tickets_.removeAt(row);
    endRemoveRows();
}

Ticket TicketTableModel::getTicket(int row) const {
    return tickets_.at(row);
}
