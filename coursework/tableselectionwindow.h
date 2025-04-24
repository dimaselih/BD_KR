#ifndef TABLESELECTIONWINDOW_H
#define TABLESELECTIONWINDOW_H

#include <QMainWindow>
#include "workexperiencewindow.h"
#include "educationwindow.h"
#include "personnelwindow.h"
#include "departmentwindow.h"
#include "positionwindow.h"
#include "assignmentwindow.h"

namespace Ui {
class TableSelectionWindow;
}

class TableSelectionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TableSelectionWindow(QWidget *parent = nullptr);
    ~TableSelectionWindow();

private slots:
    void on_btnWorkExperience_clicked();
    void on_btnEducation_clicked();
    void on_btnPersonnel_clicked();
    void on_btnDepartment_clicked();
    void on_btnPosition_clicked();
    void on_btnAssignment_clicked();
    void on_btnBack_clicked();

private:
    Ui::TableSelectionWindow *ui;
    WorkExperienceWindow *workExperienceWindow;
    EducationWindow *educationWindow;
    PersonnelWindow *personnelWindow;
    DepartmentWindow *departmentWindow;
    PositionWindow *positionWindow;
    AssignmentWindow *assignmentWindow;
};

#endif // TABLESELECTIONWINDOW_H
