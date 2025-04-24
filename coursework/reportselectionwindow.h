#ifndef REPORTSELECTIONWINDOW_H
#define REPORTSELECTIONWINDOW_H

#include <QMainWindow>
#include "workexperiencereportwindow.h"
#include "educationreportwindow.h"
#include "personnelreportwindow.h"
#include "departmentreportwindow.h"
#include "positionreportwindow.h"
#include "assignmentreportwindow.h"

namespace Ui {
class ReportSelectionWindow;
}

class ReportSelectionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ReportSelectionWindow(QWidget *parent = nullptr);
    ~ReportSelectionWindow();

private slots:
    void on_btnWorkExperienceReport_clicked();
    void on_btnEducationReport_clicked();
    void on_btnPersonnelReport_clicked();
    void on_btnDepartmentReport_clicked();
    void on_btnPositionReport_clicked();
    void on_btnAssignmentReport_clicked();
    void on_btnBack_clicked();

private:
    Ui::ReportSelectionWindow *ui;
    WorkExperienceReportWindow *workExperienceReportWindow;
    EducationReportWindow *educationReportWindow;
    PersonnelReportWindow *personnelReportWindow;
    DepartmentReportWindow *departmentReportWindow;
    PositionReportWindow *positionReportWindow;
    AssignmentReportWindow *assignmentReportWindow;
};

#endif // REPORTSELECTIONWINDOW_H
