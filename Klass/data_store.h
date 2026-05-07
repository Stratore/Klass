#ifndef DATASTORE_H
#define DATASTORE_H

#include <QString>
#include <QList>
#include "student.h" // On vérifie que c'est bien student.h

class DataStore
{
public:
    DataStore();

    // JSON
    static QList<Student*> loadFromJson(const QString &filePath);
    static bool saveToJson(const QString &filePath, const QList<Student*> &data);

    // XML
    static QList<Student*> loadFromXml(const QString &filePath);
    static bool saveToXml(const QString &filePath, const QList<Student*> &data);
};

#endif // DATASTORE_H
