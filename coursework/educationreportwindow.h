#ifndef EDUCATIONREPORTWINDOW_H
#define EDUCATIONREPORTWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QComboBox>

namespace Ui {
class EducationReportWindow;
}

class EducationReportWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EducationReportWindow(QWidget *parent = nullptr);
    ~EducationReportWindow();

private slots:
    void on_btnBack_clicked();
    void on_btnSearch_clicked();

private:
    Ui::EducationReportWindow *ui;

    void loadData(int personnelId = -1);
    void loadPersonnelComboBox();
};

#endif // EDUCATIONREPORTWINDOW_H
