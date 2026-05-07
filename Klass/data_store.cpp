#include "data_store.h"
#include "student.h"
#include "absence.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

DataStore::DataStore() {}

// JSON
bool DataStore::saveToJson(const QString &filePath, const QList<Student*> &data)
{
    QJsonArray studentArray;

    for (const Student *s : data) {
        QJsonObject sObj;
        // Utilisation des nouveaux getters de ta classe Student
        sObj["lastName"] = s->getLastName();
        sObj["firstName"] = s->getFirstName();
        sObj["birthDate"] = s->getBirthDate().toString(Qt::ISODate);
        sObj["studentNumber"] = s->getStudentNumber();
        sObj["ineNumber"] = s->getIneNumber();
        sObj["personalPhone"] = s->getPersonalPhone();
        sObj["amuEmail"] = s->getAmuEmail();
        sObj["personalEmail"] = s->getPersonalEmail();
        sObj["personalAddress"] = s->getPersonalAddress();
        sObj["parentsAddress"] = s->getParentsAddress();
        sObj["section"] = static_cast<int>(s->getSection());

        QJsonArray absenceArray;
        for (const absence &abs : s->getAbsences()) {
            QJsonObject absObj;
            absObj["date"] = abs.getDate().toString(Qt::ISODate);
            absObj["justified"] = abs.isJustified();
            absObj["reason"] = abs.reason();
            absenceArray.append(absObj);
        }
        sObj["absences"] = absenceArray;
        studentArray.append(sObj);
    }

    QJsonDocument doc(studentArray);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) return false;

    file.write(doc.toJson());
    file.close();
    return true;
}

QList<Student*> DataStore::loadFromJson(const QString &filePath)
{
    QList<Student*> list;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return list;

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray array = doc.array();

    for (int i = 0; i < array.size(); ++i) {
        QJsonObject obj = array[i].toObject();
        Student *s = new Student();

        // Utilisation des nouveaux setters de ta classe Student
        s->setLastName(obj["lastName"].toString());
        s->setFirstName(obj["firstName"].toString());
        s->setBirthDate(QDate::fromString(obj["birthDate"].toString(), Qt::ISODate));
        s->setStudentNumber(obj["studentNumber"].toString());
        s->setIneNumber(obj["ineNumber"].toString());
        s->setPersonalPhone(obj["personalPhone"].toString());
        s->setAmuEmail(obj["amuEmail"].toString());
        s->setPersonalEmail(obj["personalEmail"].toString());
        s->setPersonalAddress(obj["personalAddress"].toString());
        s->setParentsAddress(obj["parentsAddress"].toString());
        s->setSection(static_cast<Student::Section>(obj["section"].toInt()));

        QJsonArray absenceArray = obj["absences"].toArray();
        for (int j = 0; j < absenceArray.size(); ++j) {
            QJsonObject absObj = absenceArray[j].toObject();
            absence abs;
            abs.setDate(QDate::fromString(absObj["date"].toString(), Qt::ISODate));
            abs.setJustified(absObj["justified"].toBool());
            abs.setReason(absObj["reason"].toString());
            s->addAbsence(abs);
        }
        list.append(s);
    }
    file.close();
    return list;
}

// XML (Squelette aligné sur Student)
bool DataStore::saveToXml(const QString &filePath, const QList<Student*> &data) { return false; }
QList<Student*> DataStore::loadFromXml(const QString &filePath) { return QList<Student*>(); }
