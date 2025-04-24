#ifndef WORKEXPERIENCEREPORTWINDOW_H
#define WORKEXPERIENCEREPORTWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QComboBox>

namespace Ui {
class WorkExperienceReportWindow;
}

class WorkExperienceReportWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WorkExperienceReportWindow(QWidget *parent = nullptr);
    ~WorkExperienceReportWindow();

private slots:
    void on_btnBack_clicked();
    void on_btnSearch_clicked();

private:
    Ui::WorkExperienceReportWindow *ui;

    void loadData(int personnelId = -1);
    void loadPersonnelComboBox();
};

#endif // WORKEXPERIENCEREPORTWINDOW_H
