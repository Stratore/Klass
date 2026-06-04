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


    this->setWindowTitle("Klass");  // petit jeu de mot pour le titre

    ui->tableWidget_absence->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);    // pour que le qtablewidget soit en entier

    //config de la taille des icônes de la liste
    ui->listWidget_student->setIconSize(QSize(40, 40));

    // rempplissage de la ComboBox
    ui->comboBox_section->addItem(tr("Info BUT 1"));
    ui->comboBox_section->addItem(tr("Info BUT 2"));
    ui->comboBox_section->addItem(tr("Info BUT 3"));
    ui->comboBox_section->addItem(tr("MMI BUT 1"));
    ui->comboBox_section->addItem(tr("MMI BUT 2"));
    ui->comboBox_section->addItem(tr("MMI BUT 3"));

    translator = nullptr;   // on initialise un objet traslator pour pourvoire traduir plus tard

}

void MainWindow::setupInterfaceWithStudents()
{
    //nettoie la liste graphique actuelle au cas où un fichier était déjà ouvert
    ui->listWidget_student->clear();
    QStringList listeNoms;

    for (int i = 0; i < liste_of_students.size(); ++i) {    // pour chaques étudiants
        Student *s = liste_of_students[i];
        QString texteStud = s->getFirstName() + " " + s->getLastName() + tr(" (N° ") + s->getStudentNumber() + tr(")"); // les infos qu'on a trouvé les plus importantes
        QListWidgetItem *stud = new QListWidgetItem();

        if (!s->getIdPhoto().isNull()) {
            stud->setIcon(QIcon(QPixmap::fromImage(s->getIdPhoto())));  // si il y a un image on la charge
        } else {
            stud->setIcon(QIcon("../../images/default.jpg"));   // sinon on en a prévus une dans le fichier par défaut
        }

        ui->listWidget_student->addItem(stud);  // on rajoute le la case dans le liste
        stud->setText(texteStud);   // et on y met les infos principale de l'étudiant pour que l'utilisateur sache de qui on parle

        listeNoms.push_back(texteStud); // pour pouvoire l'utiliser pour le QCompleter comme ca peut importe si on tape le nom, prenenom ou, id ca peut recomander
    }

    //config dynamique du QCompleter avec le nouveau fichier listeNoms
    QCompleter *completer = new QCompleter(listeNoms, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive); // il ne prend pas la casse
    ui->lineEdit_search_student->setCompleter(completer);

}

void MainWindow::on_actionTrad_triggered()  // bouton de trad
{
    static bool estEnFrancais = false;  // vu qu'il y a un seul bouton pout traduire de fr a us il faut savoir qans qu'elle langue on est

    if (!estEnFrancais) {
        if (translator == nullptr) {
            translator = new QTranslator(this); // création d'un nouvelle objet
        }

        if (translator->load(":/i18n/Klass_fr_FR")) {   // vérif si on trouve bien le fichier
            qApp->installTranslator(translator);        //dans le tp on l'implémente dans le main avec a-> mais la il faut passer par qApp
            estEnFrancais = true;
            qDebug() << "[TRAD] French loaded from resources.";
        } else {
            qWarning() << "[TRAD] Failed to load :/i18n/Klass_fr_FR.";
        }
    }
    else {
        if (translator != nullptr) {
            qApp->removeTranslator(translator);     // on remove l'objet comme ca ca remet en anglais
        }
        estEnFrancais = false;
        qDebug() << "[TRAD] Switched back to English.";
    }

    ui->retranslateUi(this);            // il est néssésaire de refresh l'app
    setWindowTitle("Klass");            // car sinon qt remet le nom par defaut

    // si un étudiant est bien sélectionné on force le rafraîchissement de sa fiche pour traduire instantanément ses informations (par ex pour le statut Current/Modified)
    int currentRow = ui->listWidget_student->currentRow();
    if (currentRow >= 0 && currentRow < liste_of_students.size()) {
        on_listWidget_student_currentRowChanged(currentRow);
    }
}



void MainWindow::on_actionOpen_JSON_triggered() // pour load le JSON
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

void MainWindow::on_actionopen_XML_triggered()  // pour load le XML
{
    QString cheminFichier = QFileDialog::getOpenFileName(this,
                                                         tr("Ouvrir le fichier de données XML"),
                                                         "../../src/",
                                                         tr("Fichiers XML (*.xml)"));

    if (cheminFichier.isEmpty()) {
        return;
    }

    liste_of_students = DataStore::loadFromXml(cheminFichier);

    setupInterfaceWithStudents();

    ui->label_issues->setText(tr("XML loaded successfully."));




}

void MainWindow::clear() {      // vu qu'on va etre amené a faire plusieur clear j'en fait une fonction pour pas fair de redondances

    ui->label_issues->text().clear();   // pour netoyer le message d'erreur

    ui->listWidget_student->setCurrentRow(-1); // on vide le listWidget pour désélectionner l'étudiant en cours

    // on nettoie tous les LineEdit du formulaire
    ui->lineEdit_profil->clear();
    ui->lineEdit_first_name->clear();
    ui->lineEdit_last_name->clear();
    ui->lineEdit_id->clear();
    ui->lineEdit_ine->clear();
    ui->lineEdit_amu_email->clear();
    ui->lineEdit_home_email->clear();
    ui->lineEdit_phone->clear();
    ui->lineEdit_adress->clear();

    // on remet la date de naissance par défaut (01/01/2000)
    ui->dateEdit_birth->setDate(QDate(2000, 1, 1));

    // on remet la ComboBox de la section sur le premier élément (Info BUT 1)
    ui->comboBox_section->setCurrentIndex(0);

    ui->tableWidget_absence->clearContents();   // pour clear les absences

    QPixmap defaultPix("../../images/default.jpg"); // et on remet l'image par defaut
    ui->label_photo->setPixmap(defaultPix.scaled(180, 300, Qt::KeepAspectRatio));

}

void MainWindow::on__add_student_clicked()  // pour crée un nouvelle élève on va juste devoir remplir les cases vides et sauvegarder
{
    clear();
    ui->lineEdit_last_name->setFocus(); //pour que le curseur soi directemtn sur lineEdite_last_name
    ui->lineEdit_status->setText(tr("Made"));   // on rafraichi le statut pour dire qu'on est entrain de faire

}


void MainWindow::on_pushButton_add_absence_clicked()    // le bouton d'ajoue d'abscence
{
    // vérif qu'un étudiant est bien sélectionné dans la liste
    int currentRow = ui->listWidget_student->currentRow();
    if (currentRow < 0 || currentRow >= liste_of_students.size()) {
        ui->label_issues->setText(tr("Error: Select a student before adding an absence."));
        return;
    }

    Student *s = liste_of_students[currentRow]; // l'étudiant choisi

    // on a désider de passer par un fenêtre pop up pour remplir une nouvelle abscence on trouve ca plus propre
    QDialog popup(this);
    popup.setWindowTitle(tr("Add New Absence"));
    popup.setMinimumSize(350, 200);

    QDateEdit *champDate = new QDateEdit(&popup);
    champDate->setDate(QDate::currentDate()); // met la date du jour par défaut
    champDate->setCalendarPopup(true);       // active le petit calendrier cliquable

    QComboBox *champJustified = new QComboBox(&popup);  // pas besoin de plus que ces deux choix
    champJustified->addItem(tr("not justify"));
    champJustified->addItem(tr("Justify"));

    QLineEdit *champMotif = new QLineEdit(&popup);  // la raison
    champMotif->setPlaceholderText(tr("Reason for absence..."));

    QPushButton *boutonValider = new QPushButton(tr("Add"), &popup);

    // rangement propre dans un layout vertical
    QVBoxLayout *layout = new QVBoxLayout(&popup);
    layout->addWidget(new QLabel(tr("Date of absence:"), &popup));
    layout->addWidget(champDate);
    layout->addWidget(new QLabel(tr("Status:"), &popup));
    layout->addWidget(champJustified);
    layout->addWidget(new QLabel(tr("Reason:"), &popup));
    layout->addWidget(champMotif);
    layout->addWidget(boutonValider);

    // on connect le bouton àala validation de la popup
    connect(boutonValider, &QPushButton::clicked, &popup, &QDialog::accept);

    // affichage
    if (popup.exec() == QDialog::Accepted) {

        // on crée l'objet absence avec les données saisies juste avant
        absence newAbsence;

        newAbsence.setDate(champDate->date());

        // ici si l'index de la combobox est 1, c'est Justify sinon not justify
        bool isJustified = (champJustified->currentIndex() == 1);
        newAbsence.setJustified(isJustified);

        newAbsence.setReason(champMotif->text());

        // on ajoute cette absence à notre étudiant en mémoire
        s->addAbsence(newAbsence);

        // on récupère le numéro de la nouvelle ligne
        int nouvelleLigne = ui->tableWidget_absence->rowCount();
        ui->tableWidget_absence->insertRow(nouvelleLigne);

        // on insère les 3 cases dans le tableau
        QString dateTexte = newAbsence.getDate().toString("dd/MM/yyyy");    // on convertie car le tableau veux des string
        ui->tableWidget_absence->setItem(nouvelleLigne, 0, new QTableWidgetItem(dateTexte));    //on ajoute la ligne

        QString justificationTexte = isJustified ? tr("Justify") : tr("not justify");
        ui->tableWidget_absence->setItem(nouvelleLigne, 1, new QTableWidgetItem(justificationTexte));

        ui->tableWidget_absence->setItem(nouvelleLigne, 2, new QTableWidgetItem(newAbsence.reason()));  // pas besoin de crée de QString car c'est deja un string

        // on change le statut de la fiche pour indiquer qu'il y a eu une modification
        ui->lineEdit_status->setText(tr("modified"));

        qDebug() << "[ABSENCE] Nouvelle absence ajoutée pour" << s->getFirstName();
    }
}


void MainWindow::on_pushButton_remove_absence_clicked() // enlever une abscence
{
    // securirité
    int currentRowStudent = ui->listWidget_student->currentRow();
    if (currentRowStudent < 0 || currentRowStudent >= liste_of_students.size()) {
        ui->label_issues->setText(tr("Error: Select a student first."));
        return;
    }

    Student *s = liste_of_students[currentRowStudent];  // l'etudiant choisi

    // on récupère la ligne sélectionnée dans le tab des absences
    int currentRowAbsence = ui->tableWidget_absence->currentRow();

    // sécurité qui vérifie que l'utilisateur a bien cliqué sur une ligne du tableau
    if (currentRowAbsence < 0) {
        ui->label_issues->setText(tr("Error: Select the absence row to remove."));
        return;
    }

    s->removeAbsence(currentRowAbsence);    // on utilise la fonction de student pour supprimer une abscence

    // il faut la sup aussi dans le QTableWidget
    ui->tableWidget_absence->removeRow(currentRowAbsence);

    // il faut chenger aussi le statut
    ui->lineEdit_status->setText(tr("modified"));

    qDebug() << "[ABSENCE] Absence supprimée à l'index" << currentRowAbsence;
}


void MainWindow::on_pushButton_update_photo_clicked()   // pour charger une photo
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                         tr("Choisir une photo de profil"),
                                                         "",
                                                         tr("Images (*.jpg *.png)"));

    // on vérifie que l'utilisateur n'a pas cliqué sur annuler
    if (filePath == "") {
        return;
    }

    // on crée la QImage à partir du fichier choisi
    QImage nouvelleImage(filePath);

    // on l'affiche dans l'interface
    ui->label_photo->setPixmap(QPixmap::fromImage(nouvelleImage).scaled(200, 300, Qt::KeepAspectRatio));

    // et on l'enregistre dans l'objet de l'étudiant actuellement sélectionné
    int currentRow = ui->listWidget_student->currentRow();
    if (currentRow >= 0 && currentRow < liste_of_students.size()) {
        Student *s = liste_of_students[currentRow];
        s->setIdPhoto(nouvelleImage);
    }
}

void MainWindow::on_lineEdit_search_student_returnPressed() // la recherche
{
    ui->label_issues->clear();

    QString textSearch = ui->lineEdit_search_student->text(); //permet de récupérer le texte

    //si la recherche est vide on raffiche tout le monde
    if (textSearch.isEmpty()) {
        for (int i = 0; i < ui->listWidget_student->count(); ++i) {
            ui->listWidget_student->item(i)->setHidden(false);
        }
        return;
    }

    //crée l'objet regex avec le texte saisi, (qregularexpression correspind au moteur de recherche par expressions régulières de qt)
    QRegularExpression regex(textSearch, QRegularExpression::CaseInsensitiveOption);

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
            if (item->text().contains(textSearch, Qt::CaseInsensitive)) { //verif si le morceau de texte 'textSearch' se trouve dans l'item parcouru
                item->setHidden(false);
            } else {
                item->setHidden(true);
            }
        }
    }
}


void MainWindow::on_listWidget_student_currentRowChanged(int currentRow)    // quand on clique sur un student et ca nous donne son index en paramètre
{

    ui->label_issues->text().clear();   // pour netoyer le message d'erreur

    ui->tableWidget_absence->clearContents();   // pour enlever les contenue des absence de l'élève d'avant

    // sécurité
    if (currentRow < 0 || currentRow >= liste_of_students.size()) {
        return;
    }

    // on récupère le pointeur de l'étudiant qui correspond à la ligne cliquée
    Student *s = liste_of_students[currentRow];

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


void MainWindow::on_pushButton_save_clicked()       // le but de cette fonction est de savoir deja si on créer un nouvelle élcève ou on en modifie un
{
    // on recup les données
    QString id = ui->lineEdit_id->text();
    QString name = ui->lineEdit_last_name->text();
    QString lastename = ui->lineEdit_first_name->text();

    // sécurité
    if (id == "" || name == "" || lastename == "") {
        ui->label_issues->setText(tr("Error: The name, surname, or ID is missing."));
        return;
    }

    Student *etudiantCible = nullptr;
    int findInedex = -1;

    // on cherche si l'étudiant existe déjà         //////////////////////////////////////////////////////////////////////////////////////////// il faut que je continue ici
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
    etudiantCible->setLastName(name);
    etudiantCible->setFirstName(lastename);
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
