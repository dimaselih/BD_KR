#ifndef DEPARTMENTWINDOW_H
#define DEPARTMENTWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QTableWidgetItem>

namespace Ui {
class DepartmentWindow;
}

class DepartmentWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DepartmentWindow(QWidget *parent = nullptr);
    ~DepartmentWindow();

private slots:
    void on_btnAdd_clicked();
    void on_btnEdit_clicked();
    void on_btnDelete_clicked();
    void on_btnBack_clicked();
    void on_tableWidget_itemClicked(QTableWidgetItem *item);

private:
    Ui::DepartmentWindow *ui;

    void loadData();
};

#endif // DEPARTMENTWINDOW_H
