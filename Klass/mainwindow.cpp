#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCompleter>
#include <QListWidgetItem>

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

    liste_of_students = DataStore::loadFromJson("src/students_dataset.json");

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

        //ui->lineEdit_first_name->setText(item->get);          pour rentrer les valeurs
    }





}

