#ifndef PERSONNELREPORTWINDOW_H
#define PERSONNELREPORTWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QComboBox>

namespace Ui {
class PersonnelReportWindow;
}

class PersonnelReportWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PersonnelReportWindow(QWidget *parent = nullptr);
    ~PersonnelReportWindow();

private slots:
    void on_btnBack_clicked();
    void on_btnSearch_clicked();

private:
    Ui::PersonnelReportWindow *ui;

    void loadData(const QString &searchText = "");
    void setupGenderComboBox();
};

#endif // PERSONNELREPORTWINDOW_H
