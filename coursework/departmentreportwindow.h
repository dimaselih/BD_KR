#ifndef DEPARTMENTREPORTWINDOW_H
#define DEPARTMENTREPORTWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QComboBox>
#include <QDate>  // Add this line

namespace Ui {
class DepartmentReportWindow;
}

class DepartmentReportWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DepartmentReportWindow(QWidget *parent = nullptr);
    ~DepartmentReportWindow();

private slots:
    void on_btnBack_clicked();
    void on_btnSearch_clicked();

private:
    Ui::DepartmentReportWindow *ui;

    void loadData(int departmentId = -1);
    void loadDepartmentComboBox();
    void showDepartmentManager(int departmentId = -1);
};

#endif // DEPARTMENTREPORTWINDOW_H
