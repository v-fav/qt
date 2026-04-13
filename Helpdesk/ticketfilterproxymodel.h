#ifndef TICKETFILTERPROXYMODEL_H
#define TICKETFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class TicketFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit TicketFilterProxyModel(QObject* parent = nullptr);

    void setStatusFilter(const QString& status);
    void setPriorityFilter(const QString& priority);
    void setSearchText(const QString& text);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

private:
    QString status_;
    QString priority_;
    QString text_;
};

#endif
