#ifndef WORKEXPERIENCEWINDOW_H
#define WORKEXPERIENCEWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateEdit>
#include <QComboBox>
#include <QTableWidgetItem>

namespace Ui {
class WorkExperienceWindow;
}

class WorkExperienceWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WorkExperienceWindow(QWidget *parent = nullptr);
    ~WorkExperienceWindow();

private slots:
    void on_btnAdd_clicked();
    void on_btnEdit_clicked();
    void on_btnDelete_clicked();
    void on_btnBack_clicked();
    void on_tableWidget_itemClicked(QTableWidgetItem *item);

private:
    Ui::WorkExperienceWindow *ui;

    void loadData();
    void loadPersonnelComboBox();
};

#endif // WORKEXPERIENCEWINDOW_H
