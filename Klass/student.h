#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <QString>
#include <QImage>
#include <QDate>


class Student
{
public:
    Student();
    ~Student();

    enum Section {
        INFO_BUT1,
        INFO_BUT2,
        INFO_BUT3,
        MMI_BUT1,
        MMI_BUT2,
        MMI_BUT3
    };

    // Constructeur avec tous les paramètres
    Student(const QString &_lastName,
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
            Section _section);





    QString lastName;
    QString firstName;
    QDate birthDate;

    QString studentNumber;
    QString ineNumber;

    QString personalPhone;
    QString amuEmail;
    QString personalEmail;

    QString personalAddress;
    QString parentsAddress;

    QImage idPhoto;
    Section section;


};

#endif // STUDENT_H
