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
    qDebug() << "[JSON SAVE] Début de la sauvegarde dans :" << filePath;
    qDebug() << "[JSON SAVE] Nombre d'étudiants à sauvegarder :" << data.size();

    QJsonArray studentArray;

    for (int i = 0; i < data.size(); ++i) {
        const Student *s = data[i];

        // Sécurité : On évite de faire crasher le programme si un pointeur est nul
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
        sObj["section"] = static_cast<int>(s->getSection());
        sObj["photo"] = "../../images/image_" + s->getStudentNumber() + ".png";



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
        // qCritical est parfait pour les erreurs bloquantes
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
        return list; // Retourne une liste vide
    }

    QByteArray data = file.readAll();
    file.close();

    // Objet pour capturer les détails d'une éventuelle erreur de parsing
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    // Vérification de la validité du JSON
    if (doc.isNull()) {
        qCritical() << "[JSON LOAD] Erreur de syntaxe JSON détectée !";
        qCritical() << "[JSON LOAD] Message :" << parseError.errorString();
        qCritical() << "[JSON LOAD] À l'octet numéro :" << parseError.offset;
        return list;
    }

    // Vérification que la racine est bien un tableau (Array)
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

        // Extraction sécurisée avec vérification de la présence des clés essentielles
        // Si une clé textuelle manque, .toString() renverra simplement une chaîne vide "" au lieu de crasher
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

        QString cheminPhoto = obj["photo"].toString();
        s->setIdPhoto(QImage(cheminPhoto));

        if (obj.contains("photo")) {
            QString cheminPhoto = obj["photo"].toString();
            s->setIdPhoto(QImage(cheminPhoto));
        } else {
            s->setIdPhoto(QImage()); // Image nulle si absente du JSON
        }

        // Gestion de l'énumération de la section
        int sectionInt = obj["section"].toInt(-1); // -1 par défaut si la clé n'existe pas
        if (sectionInt >= 0 && sectionInt <= 5) {
            s->setSection(static_cast<Student::Section>(sectionInt));
        } else {
            qWarning() << "[JSON LOAD] Section invalide ou manquante pour" << s->getFirstName() << s->getLastName() << ". 'INFO_BUT1' appliqué par défaut.";
            s->setSection(Student::INFO_BUT1);
        }

        // Extraction des absences
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
bool DataStore::saveToXml(const QString &filePath, const QList<Student*> &data) { return false; }
QList<Student*> DataStore::loadFromXml(const QString &filePath) { return QList<Student*>(); }
