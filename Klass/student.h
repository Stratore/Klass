#ifndef STUDENT_H
#define STUDENT_H

#include <QString>
#include <QImage>
#include <QDate>
#include <QList>
#include "absence.h"

class Student
{
public:
    enum Section { INFO_BUT1, INFO_BUT2, INFO_BUT3, MMI_BUT1, MMI_BUT2, MMI_BUT3};     // le enum permet de sécuriser les valeurs

    Student();
    Student(const QString &_lastName, const QString &_firstName, const QDate &_birthDate,
            const QString &_studentNumber, const QString &_ineNumber, const QString &_personalPhone,
            const QString &_amuEmail, const QString &_personalEmail, const QString &_personalAddress,
            const QString &_parentsAddress, const QImage &_idPhoto, Section _section);
    ~Student();

    QString getLastName() const;
    QString getFirstName() const;
    QDate getBirthDate() const;
    QString getStudentNumber() const;
    QString getIneNumber() const;
    QString getPersonalPhone() const;
    QString getAmuEmail() const;
    QString getPersonalEmail() const;
    QString getPersonalAddress() const;
    QString getParentsAddress() const;
    QImage getIdPhoto() const;
    Section getSection() const;
    QList<absence> getAbsences() const;


    void setLastName(const QString &value);
    void setFirstName(const QString &value);
    void setBirthDate(const QDate &value);
    void setStudentNumber(const QString &value);
    void setIneNumber(const QString &value);
    void setPersonalPhone(const QString &value);
    void setAmuEmail(const QString &value);
    void setPersonalEmail(const QString &value);
    void setPersonalAddress(const QString &value);
    void setParentsAddress(const QString &value);
    void setIdPhoto(const QImage &value);
    void setSection(Section value);


    void addAbsence(const absence &abs);
    void removeAbsence(int &index);


private:
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
    QList<absence> absences;
};

#endif // STUDENT_H
