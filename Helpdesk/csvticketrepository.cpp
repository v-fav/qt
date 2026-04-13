#include "CsvTicketRepository.h"
#include <QFile>
#include <QTextStream>
#include <QSaveFile>

CsvTicketRepository::CsvTicketRepository(const QString& filePath)
    : filePath_(filePath)
{
}

QVector<Ticket> CsvTicketRepository::loadAll() const
{
    QVector<Ticket> tickets;

    QFile file(filePath_);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return tickets;

    QTextStream in(&file);

    if (!in.atEnd())
        in.readLine();

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");

        if (parts.size() < 6)
            continue;

        Ticket t;
        t.id = parts[0].toInt();
        t.title = parts[1];
        t.priority = parts[2];
        t.status = parts[3];
        t.createdAt = parts[4];
        t.description = parts[5];

        tickets.push_back(t);
    }

    return tickets;
}

bool CsvTicketRepository::saveAll(const QVector<Ticket>& tickets) const
{
    QSaveFile file(filePath_);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);

    out << "ID,Title,Priority,Status,CreatedAt,Description\n";

    for (const Ticket& t : tickets) {

        QString safeDesc = t.description;
        safeDesc.replace("\n", " ");

        out << t.id << ","
            << t.title << ","
            << t.priority << ","
            << t.status << ","
            << t.createdAt << ","
            << safeDesc << "\n";
    }

    return file.commit();
}

int CsvTicketRepository::getNextId(const QVector<Ticket>& tickets) const
{
    int maxId = 0;

    for (const Ticket& t : tickets) {
        if (t.id > maxId)
            maxId = t.id;
    }

    return maxId + 1;
}
