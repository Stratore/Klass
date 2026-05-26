#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCompleter>
#include <QListWidgetItem>
#include <QPixmap>

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

    for (Student *s : liste_of_students) {

        QString texteStud = s->getFirstName() + " " + s->getLastName() + " (N° " + s->getStudentNumber() + ")";
        QListWidgetItem *stud = new QListWidgetItem();
        stud->setText(texteStud);
        ui->listWidget_student->addItem(stud);

        listeNoms.push_back(texteStud);
    }

    listeNoms.sort();   // tri par ordre croissant

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


void MainWindow::on__add_student_clicked()
{

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
    // sécurité
    if (currentRow < 0 || currentRow >= liste_of_students.size()) {
        return;
    }

    // on récupère le pointeur de l'étudiant qui correspond à la ligne cliquée
    Student *s = liste_of_students[currentRow];

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

    int index = ui->comboBox_section->currentIndex();
    s->setSection(Student::Section(index));

    // photo de profil
    if (!s->getIdPhoto().isNull()) {
        ui->label_photo->setPixmap(QPixmap::fromImage(s->getIdPhoto()).scaled(120, 120, Qt::KeepAspectRatio));
    } else {
        ui->label_photo->setText("Pas de photo");
    }
}

