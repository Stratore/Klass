#ifndef ABSENCE_H
#define ABSENCE_H

#include <QDate>
#include <QString>

class absence
{
public:
    absence();

    //getters
    QDate getDate() const;
    bool isJustified() const;
    QString reason() const;

    //setters
    void setDate(const QDate &date);
    void setJustified(bool justified);
    void setReason(const QString &reason);

private:
    QDate abs_date;
    bool abs_justified;
    QString abs_reason;
};

#endif // ABSENCE_H
