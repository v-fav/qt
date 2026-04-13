#ifndef TICKET_H
#define TICKET_H

#include <QString>

struct Ticket {
    int id;
    QString title;
    QString priority;
    QString status;
    QString createdAt;
};

#endif
