#ifndef PERSONNELWINDOW_H
#define PERSONNELWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateEdit>
#include <QComboBox>
#include <QTableWidgetItem>

namespace Ui {
class PersonnelWindow;
}

class PersonnelWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PersonnelWindow(QWidget *parent = nullptr);
    ~PersonnelWindow();

private slots:
    void on_btnAdd_clicked();
    void on_btnEdit_clicked();
    void on_btnDelete_clicked();
    void on_btnBack_clicked();
    void on_tableWidget_itemClicked(QTableWidgetItem *item);

private:
    Ui::PersonnelWindow *ui;

    void loadData();
    void setupGenderComboBox();
};

#endif // PERSONNELWINDOW_H
