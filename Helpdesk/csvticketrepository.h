#ifndef CSVTICKETREPOSITORY_H
#define CSVTICKETREPOSITORY_H

#include <QString>
#include <QVector>
#include "Ticket.h"

class CsvTicketRepository
{
public:
    explicit CsvTicketRepository(const QString& filePath);

    QVector<Ticket> loadAll() const;
    bool saveAll(const QVector<Ticket>& tickets) const;

    int getNextId(const QVector<Ticket>& tickets) const;

private:
    QString filePath_;
};

#endif
