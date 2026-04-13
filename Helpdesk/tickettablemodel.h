#ifndef TICKETTABLEMODEL_H
#define TICKETTABLEMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include "ticket.h"

class TicketTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    enum Column {
        IdCol,
        TitleCol,
        PriorityCol,
        StatusCol,
        CreatedCol,
        ColCount
    };

    explicit TicketTableModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = {}) const override;
    int columnCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void addTicket(const Ticket& ticket);
    void updateTicket(int row, const Ticket& ticket);
    void removeTicket(int row);

    Ticket getTicket(int row) const;

private:
    QVector<Ticket> tickets_;
};

#endif
