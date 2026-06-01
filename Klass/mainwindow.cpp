#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCompleter>
#include <QListWidgetItem>
#include <QPixmap>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>

#include <QRegularExpression> // pour recherche rationnelle


MainWindow::~MainWindow()
{
    delete ui;
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    this->setWindowTitle("Klass");  //définition du titre

    ui->tableWidget_absence->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);    // pour que le qtablewidget soit en entier

    //config simplement la taille des icônes de la liste
    ui->listWidget_student->setIconSize(QSize(40, 40));

    //rempplissage de la ComboBox (pour demarrage)
    ui->comboBox_section->addItem(tr("Info BUT 1"));
    ui->comboBox_section->addItem(tr("Info BUT 2"));
    ui->comboBox_section->addItem(tr("Info BUT 3"));
    ui->comboBox_section->addItem(tr("MMI BUT 1"));
    ui->comboBox_section->addItem(tr("MMI BUT 2"));
    ui->comboBox_section->addItem(tr("MMI BUT 3"));

    //pret pour avoir un fichier

    translator = nullptr;

}

void MainWindow::setupInterfaceWithStudents()
{
    //nettoie la liste graphique actuelle au cas où un fichier était déjà ouvert
    ui->listWidget_student->clear();
    QStringList listeNoms;

    for (int i = 0; i < liste_of_students.size(); ++i) {
        Student *s = liste_of_students[i];
        QString texteStud = s->getFirstName() + " " + s->getLastName() + tr(" (N° ") + s->getStudentNumber() + tr(")");
        QListWidgetItem *stud = new QListWidgetItem();

        if (!s->getIdPhoto().isNull()) {
            stud->setIcon(QIcon(QPixmap::fromImage(s->getIdPhoto())));
        } else {
            stud->setIcon(QIcon("../../images/default.jpg"));
        }

        ui->listWidget_student->addItem(stud);
        stud->setText(texteStud);

        listeNoms.push_back(texteStud);
    }

    //config dynamique du QCompleter avec le nouveau fichier chargé
    QCompleter *completer = new QCompleter(listeNoms, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEdit_search_student->setCompleter(completer);

}

void MainWindow::on_actionTrad_triggered()
{
    static bool estEnFrancais = false;

    if (!estEnFrancais) {
        if (translator == nullptr) {
            translator = new QTranslator(this);
        }

        if (translator->load(":/i18n/Klass_fr_FR")) {
            qApp->installTranslator(translator);        //dans le tp on l'implémente dans le main avec a-> mais la il faut passer par qApp
            estEnFrancais = true;
            qDebug() << "[TRAD] French loaded from resources.";
        } else {
            qWarning() << "[TRAD] Failed to load :/i18n/Klass_fr_FR.";
        }
    }
    else {
        if (translator != nullptr) {
            qApp->removeTranslator(translator);
        }
        estEnFrancais = false;
        qDebug() << "[TRAD] Switched back to English.";
    }

    ui->retranslateUi(this);            // il est néssésaire de refresh l'app
    setWindowTitle("Klass");            // car sinon qt remet le nom par defaut

    // Si un étudiant est bien sélectionné on force le rafraîchissement de sa fiche pour traduire instantanément ses informations (comme le statut "Current"/"Modified")
    int currentRow = ui->listWidget_student->currentRow();
    if (currentRow >= 0 && currentRow < liste_of_students.size()) {
        on_listWidget_student_currentRowChanged(currentRow);
    }
}



void MainWindow::on_actionOpen_JSON_triggered()
{
    QString cheminFichier = QFileDialog::getOpenFileName(this,
                                                         tr("Ouvrir le fichier de données JSON"),
                                                         "../../src/",
                                                         tr("Fichiers JSON (*.json)"));

    if (cheminFichier.isEmpty()) {
        return;
    }

    //charge les données depuis le fichier choisi
    liste_of_students = DataStore::loadFromJson(cheminFichier);

    //update l'interface graphique
    setupInterfaceWithStudents();

    ui->label_issues->setText(tr("JSON loaded successfully."));
}

void MainWindow::on_actionopen_XML_triggered()
{
    QString cheminFichier = QFileDialog::getOpenFileName(this,
                                                         tr("Ouvrir le fichier de données XML"),
                                                         "../../src/",
                                                         tr("Fichiers XML (*.xml)"));

    if (cheminFichier.isEmpty()) {
        return;
    }

    //charge les données depuis le fichier choisi
    liste_of_students = DataStore::loadFromXml(cheminFichier);

    //update l'interface graphique
    setupInterfaceWithStudents();

    ui->label_issues->setText(tr("XML loaded successfully."));




}

void MainWindow::clear() {      // vu qu'on va etre amené a faire plusieur clear j'en fait une fonction pour pas fair de redondances
    // on vide le listWidget pour désélectionner l'étudiant en cours

    ui->label_issues->text().clear();   // pour netoyer le message d'erreur

    ui->listWidget_student->setCurrentRow(-1);

    // On nettoie tous les LineEdit du formulaire
    ui->lineEdit_profil->clear();
    ui->lineEdit_first_name->clear();
    ui->lineEdit_last_name->clear();
    ui->lineEdit_id->clear();
    ui->lineEdit_ine->clear();
    ui->lineEdit_amu_email->clear();
    ui->lineEdit_home_email->clear();
    ui->lineEdit_phone->clear();
    ui->lineEdit_adress->clear();

    // on remet la date de naissance par défaut (ex: 01/01/2000)
    ui->dateEdit_birth->setDate(QDate(2000, 1, 1));

    // on remet la ComboBox de la section sur le premier élément (Info BUT 1)
    ui->comboBox_section->setCurrentIndex(0);

    ui->tableWidget_absence->clearContents();   // pour clear les absences

    QPixmap defaultPix("../../images/default.jpg");
    if (!defaultPix.isNull()) {
        ui->label_photo->setPixmap(defaultPix.scaled(180, 300, Qt::KeepAspectRatio));
    } else {
        ui->label_photo->setText(tr("Pas de photo"));
    }
}

void MainWindow::on__add_student_clicked()  // pour crée un nouvelle élève on va juste devoir remplir les cas evide et sauvegarder
{
    clear();
    ui->lineEdit_last_name->setFocus(); //pour que le curseur soi directemtn sur lineEdite_last_name
    ui->lineEdit_status->setText(tr("Made"));

}


void MainWindow::on_pushButton_add_absence_clicked()
{
    // Sécurité vérif qu'un étudiant est bien sélectionné dans la liste
    int currentRow = ui->listWidget_student->currentRow();
    if (currentRow < 0 || currentRow >= liste_of_students.size()) {
        ui->label_issues->setText(tr("Error: Select a student before adding an absence."));
        return;
    }

    Student *s = liste_of_students[currentRow];

    // Création de la fenêtre popup à la volée pour que le rentrage d
    QDialog popup(this);
    popup.setWindowTitle(tr("Add New Absence"));
    popup.setMinimumSize(350, 200);

    // Création des composants de saisie
    QDateEdit *champDate = new QDateEdit(&popup);
    champDate->setDate(QDate::currentDate()); // Met la date du jour par défaut
    champDate->setCalendarPopup(true);       // Active le petit calendrier cliquable

    QComboBox *champJustified = new QComboBox(&popup);
    champJustified->addItem(tr("not justify"));
    champJustified->addItem(tr("Justify"));

    QLineEdit *champMotif = new QLineEdit(&popup);
    champMotif->setPlaceholderText(tr("Reason for absence..."));

    QPushButton *boutonValider = new QPushButton(tr("Add"), &popup);

    // Rangement propre dans un layout vertical
    QVBoxLayout *layout = new QVBoxLayout(&popup);
    layout->addWidget(new QLabel(tr("Date of absence:"), &popup));
    layout->addWidget(champDate);
    layout->addWidget(new QLabel(tr("Status:"), &popup));
    layout->addWidget(champJustified);
    layout->addWidget(new QLabel(tr("Reason:"), &popup));
    layout->addWidget(champMotif);
    layout->addWidget(boutonValider);

    // Connexion du bouton à la validation de la popup
    connect(boutonValider, &QPushButton::clicked, &popup, &QDialog::accept);

    // Affichage de la popup
    if (popup.exec() == QDialog::Accepted) {

        // L'utilisateur a validé alors on crée l'objet absence avec les données saisies
        absence nvlAbsence;
        nvlAbsence.setDate(champDate->date());

        // Si l'index de la combobox est 1, c'est Justify (true), sinon not justify (false)
        bool estJustifie = (champJustified->currentIndex() == 1);
        nvlAbsence.setJustified(estJustifie);

        nvlAbsence.setReason(champMotif->text());

        // On ajoute cette absence à notre étudiant en mémoire
        s->addAbsence(nvlAbsence);

        // reset la tab
        // On récupère le numéro de la nouvelle ligne (la dernière)
        int nouvelleLigne = ui->tableWidget_absence->rowCount();
        ui->tableWidget_absence->insertRow(nouvelleLigne);

        // On insère les 3 cases dans le tableau
        QString dateTexte = nvlAbsence.getDate().toString("dd/MM/yyyy");
        ui->tableWidget_absence->setItem(nouvelleLigne, 0, new QTableWidgetItem(dateTexte));

        QString justificationTexte = estJustifie ? tr("Justify") : tr("not justify");
        ui->tableWidget_absence->setItem(nouvelleLigne, 1, new QTableWidgetItem(justificationTexte));

        ui->tableWidget_absence->setItem(nouvelleLigne, 2, new QTableWidgetItem(nvlAbsence.reason()));

        // On change le statut de la fiche pour indiquer qu'il y a eu une modification
        ui->lineEdit_status->setText(tr("modified"));

        qDebug() << "[ABSENCE] Nouvelle absence ajoutée pour" << s->getFirstName();
    }
}


void MainWindow::on_pushButton_remove_absence_clicked()
{
    //Sécurité On vérifie qu'un étudiant est bien sélectionné
    int currentRowStudent = ui->listWidget_student->currentRow();
    if (currentRowStudent < 0 || currentRowStudent >= liste_of_students.size()) {
        ui->label_issues->setText(tr("Error: Select a student first."));
        return;
    }

    Student *s = liste_of_students[currentRowStudent];

    // On récupère la ligne sélectionnée dans le tab des absences
    int currentRowAbsence = ui->tableWidget_absence->currentRow();

    // Sécurité, On vérifie que l'utilisateur a bien cliqué sur une ligne du tableau
    if (currentRowAbsence < 0) {
        ui->label_issues->setText(tr("Error: Select the absence row to remove."));
        return;
    }

    s->removeAbsence(currentRowAbsence);

    // Suppression dans le QTableWidget
    ui->tableWidget_absence->removeRow(currentRowAbsence);

    // On change le statut pour indiquer qu'il faut sauvegarder
    ui->lineEdit_status->setText(tr("modified"));

    qDebug() << "[ABSENCE] Absence supprimée à l'index" << currentRowAbsence;
}


void MainWindow::on_pushButton_update_photo_clicked()
{
    QString cheminFichier = QFileDialog::getOpenFileName(this,      // de la meme manière qu'on a vu dans un tp de qt
                                                         tr("Choisir une photo de profil"),
                                                         "",
                                                         tr("Images (*.jpg *.png)"));

    // Sécurité : On vérifie que l'utilisateur n'a pas cliqué sur Annuler
    if (cheminFichier == "") {
        return;
    }

    // On crée la QImage à partir du fichier choisi
    QImage nouvelleImage(cheminFichier);

    // On l'affiche immédiatement dans l'interface
    ui->label_photo->setPixmap(QPixmap::fromImage(nouvelleImage).scaled(200, 300, Qt::KeepAspectRatio));

    // On l'enregistre dans l'objet de l'étudiant actuellement sélectionné
    int currentRow = ui->listWidget_student->currentRow();
    if (currentRow >= 0 && currentRow < liste_of_students.size()) {
        Student *s = liste_of_students[currentRow];
        s->setIdPhoto(nouvelleImage);
    }
}

void MainWindow::on_lineEdit_search_student_returnPressed() // quand l'utilisateur mes un nom et appui sur ENTRE pour selcetionner
{
    ui->label_issues->clear();

    QString texteRecherche = ui->lineEdit_search_student->text(); //permet de récupérer le texte

    //si la recherche est vide, on raffiche tout le monde
    if (texteRecherche.isEmpty()) {
        for (int i = 0; i < ui->listWidget_student->count(); ++i) {
            ui->listWidget_student->item(i)->setHidden(false);
        }
        return;
    }

    //crée l'objet regex avec le texte saisi, (qregularexpression correspind au moteur de recherche par expressions régulières de qt)
    QRegularExpression regex(texteRecherche, QRegularExpression::CaseInsensitiveOption);

    //on vérifie si l'expression régulière est valide
    if (regex.isValid()) {

        for (int i = 0; i < ui->listWidget_student->count(); ++i) { // boucle sur a liste d'étudiants
            QListWidgetItem *item = ui->listWidget_student->item(i); //pointeur vers l'élément affiché a l'écran

            // QRegularExpressionMatch verifie si regex match avec le texte de l'item
            QRegularExpressionMatch match = regex.match(item->text());

            // verifie si il correspond bien au motif de regex
            if (match.hasMatch()) {
                item->setHidden(false); // pour le laisser visible
            } else {
                item->setHidden(true); // rend invisible
            }
        }
    }
    else {
        // méthode de base de recherche

        for (int i = 0; i < ui->listWidget_student->count(); ++i) {
            QListWidgetItem *item = ui->listWidget_student->item(i); // recup l'étudiant

            // qtcaseinsensitive permet d'ignorer les majuscules et les minuscules
            if (item->text().contains(texteRecherche, Qt::CaseInsensitive)) { //verif si le morceau de texte 'texteRecherche' se trouve dans l'item parcouru
                item->setHidden(false);
            } else {
                item->setHidden(true);
            }
        }
    }
}


void MainWindow::on_listWidget_student_currentRowChanged(int currentRow)
{

    ui->label_issues->text().clear();   // pour netoyer le message d'erreur

    // sécurité
    if (currentRow < 0 || currentRow >= liste_of_students.size()) {
        return;
    }

    // on récupère le pointeur de l'étudiant qui correspond à la ligne cliquée
    Student *s = liste_of_students[currentRow];

    // test
    //QString cheminDynamique = "../../images/image_" + s->getStudentNumber() + ".jpg";
    //QFile testFile(cheminDynamique);
    //qDebug() << "Nom :" << s->getFirstName();
    //qDebug() << "Fichier existe ?" << testFile.exists();
    //qDebug() << "Où Qt cherche l'image (chemin absolu) :" << QFileInfo(testFile).absoluteFilePath();


    // on remplit les cases de l'interface, le detail
    QString profil = s->getFirstName() + " " + s->getLastName();
    ui->lineEdit_profil->setText(profil);
    ui->lineEdit_first_name->setText(s->getFirstName());
    ui->lineEdit_last_name->setText(s->getLastName());
    ui->lineEdit_id->setText(s->getStudentNumber());
    ui->lineEdit_ine->setText(s->getIneNumber());
    ui->lineEdit_amu_email->setText(s->getAmuEmail());
    ui->lineEdit_home_email->setText(s->getPersonalEmail());
    ui->lineEdit_phone->setText(s->getPersonalPhone());
    ui->lineEdit_adress->setText(s->getPersonalAddress());
    ui->dateEdit_birth->setDate(s->getBirthDate());
    ui->comboBox_section->setCurrentIndex(s->getSection()); //on passe par l'index pour trouver le bon enum
    ui->lineEdit_status->setText(tr("Curent"));


    ui->tableWidget_absence->clearContents();   // pour enlever les contenue des absence de l'élève d'avant

    ui->tableWidget_absence->setRowCount(s->getAbsences().size());  // ajoute dynamique du nombre de lignes au nombre d'absences de cet étudiant

    for (int i = 0; i < s->getAbsences().size(); i++) {

        QString dateTexte = s->getAbsences()[i].getDate().toString("dd/MM/yyyy");
        QTableWidgetItem *date = new QTableWidgetItem(dateTexte);
        ui->tableWidget_absence->setItem(i,0,date);

        QString justificationTexte = s->getAbsences()[i].isJustified() ? tr("Justify") : tr("not justify");
        QTableWidgetItem *justified = new QTableWidgetItem(justificationTexte);
        ui->tableWidget_absence->setItem(i,1,justified);

        QTableWidgetItem *motif = new QTableWidgetItem(s->getAbsences()[i].reason());
        ui->tableWidget_absence->setItem(i,2,motif);
    }



    // photo de profil
    if (!s->getIdPhoto().isNull()) {
        ui->label_photo->setPixmap(QPixmap::fromImage(s->getIdPhoto()).scaled(200, 300, Qt::KeepAspectRatio));
    } else {
        QPixmap defaultPix("../../images/default.jpg");
        ui->label_photo->setPixmap(defaultPix.scaled(180, 300, Qt::KeepAspectRatio));
    }

}


void MainWindow::on_pushButton_save_clicked()       // le but de cette fonction est de savoir deja si on crer un nouvelle élçève ou on en modifie un
{
    //recup des données
    QString id = ui->lineEdit_id->text();
    QString nom = ui->lineEdit_last_name->text();
    QString prenom = ui->lineEdit_first_name->text();

    // sécurité
    if (id == "" || nom == "" || prenom == "") {
        ui->label_issues->setText(tr("Error: The name, surname, or ID is missing."));
        return;
    }

    Student *etudiantCible = nullptr;
    int findInedex = -1;

    // on cherche si l'étudiant existe déjà
    for (int i = 0; i < liste_of_students.size(); ++i) {
        if (id == liste_of_students[i]->getStudentNumber()) {
            etudiantCible = liste_of_students[i]; // Mode modification
            findInedex = i;
            ui->lineEdit_status->setText(tr("modified"));  //on change le statut
            break;

        }
    }

    // Si on ne l'a pas trouvé dans la boucle, on en crée un nouveau
    if (etudiantCible == nullptr) {
        etudiantCible = new Student();
        etudiantCible->setStudentNumber(id);
        liste_of_students.push_back(etudiantCible); // Ajout à la liste
        ui->lineEdit_status->setText(tr("Made"));
    }

    // On remplit (ou modifie) les données avec les lineEdit
    etudiantCible->setLastName(nom);
    etudiantCible->setFirstName(prenom);
    etudiantCible->setIneNumber(ui->lineEdit_ine->text());
    etudiantCible->setAmuEmail(ui->lineEdit_amu_email->text());
    etudiantCible->setPersonalEmail(ui->lineEdit_home_email->text());
    etudiantCible->setPersonalPhone(ui->lineEdit_phone->text());
    etudiantCible->setPersonalAddress(ui->lineEdit_adress->text());
    etudiantCible->setBirthDate(ui->dateEdit_birth->date());

    // On vérifie si le label a bien une image affichée
    if (!ui->label_photo->pixmap().isNull()) {
        etudiantCible->setIdPhoto(ui->label_photo->pixmap().toImage());
    } else {
        etudiantCible->setIdPhoto(QImage()); // Image vide si rien dans le label
    }

    int indexSection = ui->comboBox_section->currentIndex();
    etudiantCible->setSection(static_cast<Student::Section>(indexSection));

    // si l'étudiant a une photo valide, on l'enregistre physiquement sur le disque dur
    if (!etudiantCible->getIdPhoto().isNull() && findInedex != -1) {
        QString cheminSauvegardePhysique = "../../images/image_" + QString::number(findInedex + 1) + ".jpg";

        // on force Qt à écrire l'image sur le disque en format JPG
        bool ecritureOk = etudiantCible->getIdPhoto().save(cheminSauvegardePhysique, "JPG");

        if (ecritureOk) {
            qDebug() << "[PHOTO SAVE] L'image a bien été écrite sur le disque :" << cheminSauvegardePhysique;
        } else {
            qWarning() << "[PHOTO SAVE] Échec de l'écriture physique de l'image. Vérifie les dossiers.";
        }
    }



    // Sauvegarde automatique dans le fichier JSON avec notre fonction de save
    DataStore::saveToJson("../../src/students_dataset.json", liste_of_students);
    DataStore::saveToXml("../../src/students_dataset.xml", liste_of_students);



    // permet de mettre à jour le master a droite si on change des info plutot que de devoir sortir de l'app et revenir
    int currentRow = ui->listWidget_student->currentRow();
    if (currentRow >= 0 && currentRow < ui->listWidget_student->count()) {
        QListWidgetItem *itemActuel = ui->listWidget_student->item(currentRow);
        if (!etudiantCible->getIdPhoto().isNull()) {
            itemActuel->setIcon(QIcon(QPixmap::fromImage(etudiantCible->getIdPhoto())));
        }
        QString nouveauTexte = etudiantCible->getFirstName() + " " + etudiantCible->getLastName() + tr(" (N° ") + etudiantCible->getStudentNumber() + tr(")");
        itemActuel->setText(nouveauTexte);
    }
}


void MainWindow::on_pushButton_back_clicked()
{
    clear();        //on revien juste en arrière
}
