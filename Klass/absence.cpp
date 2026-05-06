#include "absence.h"

absence::absence()
    : abs_date(QDate::currentDate()),
    abs_justified(false),
    abs_reason("")
{
}

// Getters
QDate absence::getDate() const
{
    return abs_date;
}

bool absence::isJustified() const
{
    return abs_justified;
}

QString absence::reason() const
{
    return abs_reason;
}

// Setters
void absence::setDate(const QDate &date)
{
    abs_date = date;
}

void absence::setJustified(bool justified)
{
    abs_justified = justified;
}

void absence::setReason(const QString &reason)
{
    abs_reason = reason;
}
