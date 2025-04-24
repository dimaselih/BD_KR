#ifndef EDUCATIONWINDOW_H
#define EDUCATIONWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateEdit>
#include <QComboBox>
#include <QTableWidgetItem>

namespace Ui {
class EducationWindow;
}

class EducationWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EducationWindow(QWidget *parent = nullptr);
    ~EducationWindow();

private slots:
    void on_btnAdd_clicked();
    void on_btnEdit_clicked();
    void on_btnDelete_clicked();
    void on_btnBack_clicked();
    void on_tableWidget_itemClicked(QTableWidgetItem *item);

private:
    Ui::EducationWindow *ui;

    void loadData();
    void loadPersonnelComboBox();
};

#endif // EDUCATIONWINDOW_H
