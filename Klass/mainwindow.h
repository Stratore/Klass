#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "absence.h"
#include "data_store.h"
#include "student.h"

#include <QMainWindow>
#include <string>
#include <QString>
#include <QList>
#include <QTranslator>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void clear();
    ~MainWindow();

private slots:
    void on__add_student_clicked();

    void on_actionTrad_triggered();

    void on_pushButton_add_absence_clicked();

    void on_pushButton_remove_absence_clicked();

    void on_pushButton_update_photo_clicked();

    void on_lineEdit_search_student_returnPressed();

    void on_listWidget_student_currentRowChanged(int currentRow);

    void on_pushButton_save_clicked();

    void on_pushButton_back_clicked();

    void setupInterfaceWithStudents();

    void on_actionOpen_JSON_triggered();

    void on_actionopen_XML_triggered();

private:
    Ui::MainWindow *ui;

    QList <Student*> liste_of_students; // QList de *students pour optimiser les performances en évitant les copies lourdes (photos par ex) et permettre la modification directe en mémoire

    QTranslator *translator;

    QString m_projectRootDir;

};
#endif // MAINWINDOW_H
