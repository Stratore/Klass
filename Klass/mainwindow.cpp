#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCompleter>
#include <QListWidgetItem>
#include <QPixmap>
#include <QDebug>
#include <QFile>
#include <QFileInfo>


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

    liste_of_students = DataStore::loadFromJson("../../src/students_dataset.json");

    ui->listWidget_student->setIconSize(QSize(40, 40));

    QStringList listeNoms;

    for (int i = 0; i < liste_of_students.size(); ++i) {
        Student *s = liste_of_students[i];
        QString texteStud = s->getFirstName() + " " + s->getLastName() + " (N° " + s->getStudentNumber() + ")";
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

    //listeNoms.sort();   // tri par ordre croissant

    // le QCompleter sert a fais une liste de suggestion et j'y met ma lice triée
    QCompleter *completer = new QCompleter(listeNoms, this);

    // caseSensitive pour pas supporter la casse reconnaitre la ville meme sans majuscule,
    completer->setCaseSensitivity(Qt::CaseInsensitive);

    ui->lineEdit_search_student->setCompleter(completer);


    ui->comboBox_section->addItem("Info BUT 1");    //pour remplir le combobox
    ui->comboBox_section->addItem("Info BUT 2");
    ui->comboBox_section->addItem("Info BUT 3");
    ui->comboBox_section->addItem("MMI BUT 1");
    ui->comboBox_section->addItem("MMI BUT 2");
    ui->comboBox_section->addItem("MMI BUT 3");




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
        ui->label_photo->setText("Pas de photo");
    }
}

void MainWindow::on__add_student_clicked()  // pour crée un nouvelle élève on va juste devoir remplir les cas evide et sauvegarder
{
    clear();
    ui->lineEdit_last_name->setFocus(); //pour que le curseur soi directemtn sur lineEdite_last_name
    ui->lineEdit_status->setText("Made");

}


void MainWindow::on_pushButton_add_absence_clicked()
{

}


void MainWindow::on_pushButton_remove_absence_clicked()
{

}


void MainWindow::on_pushButton_update_photo_clicked()
{

}


void MainWindow::on_lineEdit_search_student_returnPressed() // quand l'utilisateur mes un nom et appui sur ENTRE pour selcetionner
{
    ui->label_issues->text().clear();   // pour netoyer le message d'erreur
    //pour enlever tous les autre profils dans la QListWIdget
    QString texteRecherche = ui->lineEdit_search_student->text();

    for (int i = 0; i < ui->listWidget_student->count(); ++i) {
        QListWidgetItem *item = ui->listWidget_student->item(i);

        if (item->text().contains(texteRecherche, Qt::CaseInsensitive)) {
            item->setHidden(false);
        } else {
            item->setHidden(true);
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
    //ui->lineEdit_status->setText(s;                       //savoir si c'est modified ou pas
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
    ui->lineEdit_status->setText("Curent");


    ui->tableWidget_absence->clearContents();   // pour enlever les contenue des absence de l'élève d'avant

    ui->tableWidget_absence->setRowCount(s->getAbsences().size());  // ajoute dynamique du nombre de lignes au nombre d'absences de cet étudiant

    for (int i = 0; i < s->getAbsences().size(); i++) {

        QString dateTexte = s->getAbsences()[i].getDate().toString("dd/MM/yyyy");
        QTableWidgetItem *date = new QTableWidgetItem(dateTexte);
        ui->tableWidget_absence->setItem(i,0,date);

        QString justificationTexte = s->getAbsences()[i].isJustified() ? "Justify" : "not justify";
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
        ui->label_issues->setText("Error: The name, surname, or ID is missing.");
        return;
    }

    Student *etudiantCible = nullptr;

    // on cherche si l'étudiant existe déjà
    for (int i = 0; i < liste_of_students.size(); ++i) {
        if (id == liste_of_students[i]->getStudentNumber()) {
            etudiantCible = liste_of_students[i]; // Mode modification
            ui->lineEdit_status->setText("modified");  //on change le statut
            break;

        }
    }

    // Si on ne l'a pas trouvé dans la boucle, on en crée un nouveau
    if (etudiantCible == nullptr) {
        etudiantCible = new Student();
        etudiantCible->setStudentNumber(id);
        liste_of_students.push_back(etudiantCible); // Ajout à la liste
        ui->lineEdit_status->setText("Made");
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

    int indexSection = ui->comboBox_section->currentIndex();
    etudiantCible->setSection(static_cast<Student::Section>(indexSection));

    // Sauvegarde automatique dans le fichier JSON avec notre fonction de save
    DataStore::saveToJson("../../src/students_dataset.json", liste_of_students);
}


void MainWindow::on_pushButton_back_clicked()
{
    clear();        //on revien juste en arrière
}

