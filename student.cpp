#include "student.h"

// Constructeur par défaut
Student::Student()
    : birthDate(QDate::currentDate()), section(INFO_BUT1)
{
}

// Constructeur complet
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

Student::~Student()
{
}


QString Student::getLastName() const { return lastName; }
QString Student::getFirstName() const { return firstName; }
QDate Student::getBirthDate() const { return birthDate; }
QString Student::getStudentNumber() const { return studentNumber; }
QString Student::getIneNumber() const { return ineNumber; }
QString Student::getPersonalPhone() const { return personalPhone; }
QString Student::getAmuEmail() const { return amuEmail; }
QString Student::getPersonalEmail() const { return personalEmail; }
QString Student::getPersonalAddress() const { return personalAddress; }
QString Student::getParentsAddress() const { return parentsAddress; }
QImage Student::getIdPhoto() const { return idPhoto; }
Student::Section Student::getSection() const { return section; }
QList<absence> Student::getAbsences() const { return absences; }


void Student::setLastName(const QString &value) { lastName = value; }
void Student::setFirstName(const QString &value) { firstName = value; }
void Student::setBirthDate(const QDate &value) { birthDate = value; }
void Student::setStudentNumber(const QString &value) { studentNumber = value; }
void Student::setIneNumber(const QString &value) { ineNumber = value; }
void Student::setPersonalPhone(const QString &value) { personalPhone = value; }
void Student::setAmuEmail(const QString &value) { amuEmail = value; }
void Student::setPersonalEmail(const QString &value) { personalEmail = value; }
void Student::setPersonalAddress(const QString &value) { personalAddress = value; }
void Student::setParentsAddress(const QString &value) { parentsAddress = value; }
void Student::setIdPhoto(const QImage &value) { idPhoto = value; }
void Student::setSection(Section value) { section = value; }

void Student::addAbsence(const absence &abs) {
    absences.push_back(abs);
}

void Student::removeAbsence(int &index) {
    if (index >= 0 && index < absences.size())
        absences.erase(absences.begin() + index); // supprime l'élément de la QList, absences.begin() montre l'intex 0, car on ne peut pas erase(int) il faut une adresse memoire (un itérateur)

}

