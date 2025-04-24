#ifndef ASSIGNMENTWINDOW_H
#define ASSIGNMENTWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateEdit>
#include <QComboBox>
#include <QTableWidgetItem>

namespace Ui {
class AssignmentWindow;
}

class AssignmentWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AssignmentWindow(QWidget *parent = nullptr);
    ~AssignmentWindow();

private slots:
    void on_btnAdd_clicked();
    void on_btnEdit_clicked();
    void on_btnDelete_clicked();
    void on_btnBack_clicked();
    void on_tableWidget_itemClicked(QTableWidgetItem *item);

private:
    Ui::AssignmentWindow *ui;

    void loadData();
    void loadPersonnelComboBox();
    void loadDepartmentComboBox();
    void loadPositionComboBox();
};

#endif // ASSIGNMENTWINDOW_H
