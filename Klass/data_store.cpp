#include "data_store.h"
#include "student.h"
#include "absence.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDebug>

DataStore::DataStore() {}

// JSON
bool DataStore::saveToJson(const QString &filePath, const QList<Student*> &data)
{
    qDebug() << "[JSON SAVE] Début de la sauvegarde dans :" << filePath;
    qDebug() << "[JSON SAVE] Nombre d'étudiants à sauvegarder :" << data.size();

    QJsonArray studentArray;

    for (int i = 0; i < data.size(); ++i) {
        const Student *s = data[i];

        //évite de faire crasher le programme si un pointeur est nul
        if (!s) {
            qWarning() << "[JSON SAVE] Attention : Pointeur d'étudiant nul détecté à l'index" << i << "(ignoré)";
            continue;
        }

        QJsonObject sObj;
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
        sObj["section"] = static_cast<int>(s->getSection()); // pour convertir l'enum en int. exemple: INFO_BUT1 vaut 0, MMI_BUT1 vaut 3 etc..
        sObj["photo"] = "../../images/image_" + QString::number(i + 1) + ".jpg";



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

    if (!file.open(QIODevice::WriteOnly)) {
        qCritical() << "[JSON SAVE] Erreur critique : Impossible d'ouvrir le fichier en écriture. Raison :" << file.errorString();
        return false;
    }

    qint64 bytesWritten = file.write(doc.toJson());
    file.close();

    if (bytesWritten == -1) {
        qCritical() << "[JSON SAVE] Erreur critique : Échec de l'écriture des données dans le fichier.";
        return false;
    }

    qDebug() << "[JSON SAVE] Sauvegarde réussie !" << bytesWritten << "octets écrits.";
    return true;
}



QList<Student*> DataStore::loadFromJson(const QString &filePath)
{
    QList<Student*> list;
    qDebug() << "[JSON LOAD] Tentative de chargement du fichier :" << filePath;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "[JSON LOAD] Attention : Impossible d'ouvrir le fichier en lecture. Raison :" << file.errorString();
        return list; //retourne une liste vide
    }

    QByteArray data = file.readAll();
    file.close();

    //pour verifier les détails d'une éventuelle erreur de parsing
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    //verif si le JSON fonctionne
    if (doc.isNull()) {
        qCritical() << "[JSON LOAD] Erreur de syntaxe JSON détectée !";
        qCritical() << "[JSON LOAD] Message :" << parseError.errorString();
        qCritical() << "[JSON LOAD] À l'octet numéro :" << parseError.offset;
        return list;
    }

    //verif que racine est un tableau
    if (!doc.isArray()) {
        qCritical() << "[JSON LOAD] Erreur : Le format du fichier JSON n'est pas un tableau à la racine.";
        return list;
    }

    QJsonArray array = doc.array();
    qDebug() << "[JSON LOAD] JSON valide." << array.size() << "entrées trouvées. Début de l'extraction...";

    for (int i = 0; i < array.size(); ++i) {
        if (!array[i].isObject()) {
            qWarning() << "[JSON LOAD] Élément ignoré à l'index" << i << ": Ce n'est pas un objet JSON valide.";
            continue;
        }

        QJsonObject obj = array[i].toObject();
        Student *s = new Student();

        //extract sécurisée avec vérification de la présence des clés nécessaire
        //si une clé manque, .toString() renverra simplement une chaîne vide "" au lieu de crasher
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

        //utilise l'index i de la boucle pour reconstruire "image_1.jpg", "image_2.jpg" etc...
        QString cheminPhoto = "../../images/image_" + QString::number(i + 1) + ".jpg";
        s->setIdPhoto(QImage(cheminPhoto));


        //gestion de l'énumération de la section
        int sectionInt = obj["section"].toInt(-1); // -1 par défaut si la clé n'existe pas
        if (sectionInt >= 0 && sectionInt <= 5) {
            s->setSection(static_cast<Student::Section>(sectionInt));
        } else {
            qWarning() << "[JSON LOAD] Section invalide ou manquante pour" << s->getFirstName() << s->getLastName() << ". 'INFO_BUT1' appliqué par défaut.";
            s->setSection(Student::INFO_BUT1);
        }

        //extrait les absences
        if (obj.contains("absences") && obj["absences"].isArray()) {
            QJsonArray absenceArray = obj["absences"].toArray();
            for (int j = 0; j < absenceArray.size(); ++j) {
                if (absenceArray[j].isObject()) {
                    QJsonObject absObj = absenceArray[j].toObject();
                    absence abs;
                    abs.setDate(QDate::fromString(absObj["date"].toString(), Qt::ISODate));
                    abs.setJustified(absObj["justified"].toBool(false));
                    abs.setReason(absObj["reason"].toString());
                    s->addAbsence(abs);
                }
            }
        }

        list.append(s);
    }

    qDebug() << "[JSON LOAD] Chargement terminé avec succès." << list.size() << "étudiants importés en mémoire.";
    return list;
}

// XML
bool DataStore::saveToXml(const QString &filePath, const QList<Student*> &data)
{
    qDebug() << "[XML SAVE] Début de la sauvegarde dans :" << filePath;
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical() << "[XML SAVE] Impossible d'ouvrir le fichier en écriture :" << file.errorString();
        return false;
    }

    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true); //pour avoir des retours à la ligne et une belle indentation
    xml.writeStartDocument();

    xml.writeStartElement("students");

    for (int i = 0; i < data.size(); ++i) {
        const Student *s = data[i];
        if (!s) continue;

        xml.writeStartElement("student"); // <student>

        xml.writeTextElement("lastName", s->getLastName());
        xml.writeTextElement("firstName", s->getFirstName());
        xml.writeTextElement("birthDate", s->getBirthDate().toString(Qt::ISODate));
        xml.writeTextElement("studentNumber", s->getStudentNumber());
        xml.writeTextElement("ineNumber", s->getIneNumber());
        xml.writeTextElement("personalPhone", s->getPersonalPhone());
        xml.writeTextElement("amuEmail", s->getAmuEmail());
        xml.writeTextElement("personalEmail", s->getPersonalEmail());
        xml.writeTextElement("personalAddress", s->getPersonalAddress());
        xml.writeTextElement("parentsAddress", s->getParentsAddress());
        xml.writeTextElement("section", QString::number(static_cast<int>(s->getSection())));
        xml.writeTextElement("photo", "../../images/image_" + QString::number(i + 1) + ".jpg");

        //gestion de la liste des absences
        xml.writeStartElement("absences"); // <absences>
        for (const absence &abs : s->getAbsences()) {
            xml.writeStartElement("absence"); // <absence>
            xml.writeTextElement("date", abs.getDate().toString(Qt::ISODate));
            xml.writeTextElement("justified", abs.isJustified() ? "true" : "false");
            xml.writeTextElement("reason", abs.reason());
            xml.writeEndElement(); // </absence>
        }
        xml.writeEndElement(); // </absences>

        xml.writeEndElement(); // </student>
    }

    xml.writeEndElement(); // </students>
    xml.writeEndDocument();

    file.close();
    qDebug() << "[XML SAVE] Sauvegarde XML réussie !";
    return true;
}

// chargement xml
QList<Student*> DataStore::loadFromXml(const QString &filePath)
{
    QList<Student*> list;
    qDebug() << "[XML LOAD] Tentative de chargement du fichier :" << filePath;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "[XML LOAD] Impossible d'ouvrir le fichier en lecture :" << file.errorString();
        return list;
    }

    QXmlStreamReader xml(&file);
    int studentIndex = 0;

    //on parcourt le fichier XML élément par élément
    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();

        //si on tombe sur une balise ouvrante
        if (token == QXmlStreamReader::StartElement) {
            if (xml.name() == "student") {
                Student *s = new Student();

                //on lit les éléments internes à <student> jusqu'à sa fermeture </student>
                while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "student")) {
                    xml.readNext();

                    if (xml.tokenType() == QXmlStreamReader::StartElement) {
                        QString elementName = xml.name().toString();

                        //gestion du bloc d'absences
                        if (elementName == "absences") {
                            while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "absences")) {
                                xml.readNext();
                                if (xml.tokenType() == QXmlStreamReader::StartElement && xml.name() == "absence") {
                                    absence abs;
                                    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "absence")) {
                                        xml.readNext();
                                        if (xml.tokenType() == QXmlStreamReader::StartElement) {
                                            QString absField = xml.name().toString();
                                            QString text = xml.readElementText();
                                            if (absField == "date") abs.setDate(QDate::fromString(text, Qt::ISODate));
                                            else if (absField == "justified") abs.setJustified(text == "true");
                                            else if (absField == "reason") abs.setReason(text);
                                        }
                                    }
                                    s->addAbsence(abs);
                                }
                            }
                        }
                        //gestion des champs simples de l'étudiant
                        else {
                            QString text = xml.readElementText();
                            if (elementName == "lastName") s->setLastName(text);
                            else if (elementName == "firstName") s->setFirstName(text);
                            else if (elementName == "birthDate") s->setBirthDate(QDate::fromString(text, Qt::ISODate));
                            else if (elementName == "studentNumber") s->setStudentNumber(text);
                            else if (elementName == "ineNumber") s->setIneNumber(text);
                            else if (elementName == "personalPhone") s->setPersonalPhone(text);
                            else if (elementName == "amuEmail") s->setAmuEmail(text);
                            else if (elementName == "personalEmail") s->setPersonalEmail(text);
                            else if (elementName == "personalAddress") s->setPersonalAddress(text);
                            else if (elementName == "parentsAddress") s->setParentsAddress(text);
                            else if (elementName == "section") {
                                int sectionInt = text.toInt();
                                if (sectionInt >= 0 && sectionInt <= 5) {
                                    s->setSection(static_cast<Student::Section>(sectionInt));
                                } else {
                                    s->setSection(Student::INFO_BUT1);
                                }
                            }
                        }
                    }
                }

                //chargement de la photo de l'étudiant à partir de son index
                QString cheminPhoto = "../../images/image_" + QString::number(studentIndex + 1) + ".jpg";
                s->setIdPhoto(QImage(cheminPhoto));
                studentIndex++;

                list.append(s);
            }
        }
    }

    //securite si le fichier XML est corrompu
    if (xml.hasError()) {
        qCritical() << "[XML LOAD] Erreur de parsing XML :" << xml.errorString();
    }

    file.close();
    qDebug() << "[XML LOAD] Chargement XML terminé." << list.size() << "étudiants chargés.";
    return list;
}
