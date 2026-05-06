#include "student.h"

Student::Student(const QString &_lastName,
                 const QString &_firstName,
                 const QDate &_birthDate,
                 const QString &_studentNumber,
                 const QString &_ineNumber,
                 const QString &_personalPhone,
                 const QString &_amuEmail,
                 const QString &_personalEmail,
                 const QString &_personalAddress,
                 const QString &_parentsAddress,
                 const QImage &_idPhoto,
                 Section _section)
    : lastName(_lastName),
    firstName(_firstName),
    birthDate(_birthDate),
    studentNumber(_studentNumber),
    ineNumber(_ineNumber),
    personalPhone(_personalPhone),
    amuEmail(_amuEmail),
    personalEmail(_personalEmail),
    personalAddress(_personalAddress),
    parentsAddress(_parentsAddress),
    idPhoto(_idPhoto),
    section(_section)
{
}
