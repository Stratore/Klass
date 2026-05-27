#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "absence.h"
#include "data_store.h"
#include "student.h"

#include <QMainWindow>
#include <string>
#include <QString>
#include <QList>

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

    void on_pushButton_add_absence_clicked();

    void on_pushButton_remove_absence_clicked();

    void on_pushButton_update_photo_clicked();

    void on_lineEdit_search_student_returnPressed();

    void on_listWidget_student_currentRowChanged(int currentRow);

    void on_pushButton_save_clicked();

    void on_pushButton_back_clicked();

private:
    Ui::MainWindow *ui;

    QList <Student*> liste_of_students;

};
#endif // MAINWINDOW_H
