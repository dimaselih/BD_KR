#include "reportselectionwindow.h"
#include "ui_reportselectionwindow.h"
#include <QSignalBlocker>
#include <QWidget>

ReportSelectionWindow::ReportSelectionWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ReportSelectionWindow),
    workExperienceReportWindow(nullptr),
    educationReportWindow(nullptr),
    personnelReportWindow(nullptr),
    departmentReportWindow(nullptr),
    positionReportWindow(nullptr),
    assignmentReportWindow(nullptr)
{
    ui->setupUi(this);

    // Подключаем сигналы к слотам
    connect(ui->btnWorkExperienceReport, &QPushButton::clicked, this, &ReportSelectionWindow::on_btnWorkExperienceReport_clicked);
    connect(ui->btnEducationReport, &QPushButton::clicked, this, &ReportSelectionWindow::on_btnEducationReport_clicked);
    connect(ui->btnPersonnelReport, &QPushButton::clicked, this, &ReportSelectionWindow::on_btnPersonnelReport_clicked);
    connect(ui->btnDepartmentReport, &QPushButton::clicked, this, &ReportSelectionWindow::on_btnDepartmentReport_clicked);
    connect(ui->btnPositionReport, &QPushButton::clicked, this, &ReportSelectionWindow::on_btnPositionReport_clicked);
    connect(ui->btnAssignmentReport, &QPushButton::clicked, this, &ReportSelectionWindow::on_btnAssignmentReport_clicked);
    connect(ui->btnBack, &QPushButton::clicked, this, &ReportSelectionWindow::on_btnBack_clicked);
}



ReportSelectionWindow::~ReportSelectionWindow()
{
    delete ui;
    if (workExperienceReportWindow)
        delete workExperienceReportWindow;
    if (educationReportWindow)
        delete educationReportWindow;
    if (personnelReportWindow)
        delete personnelReportWindow;
    if (departmentReportWindow)
        delete departmentReportWindow;
    if (positionReportWindow)
        delete positionReportWindow;
    if (assignmentReportWindow)
        delete assignmentReportWindow;
}



void ReportSelectionWindow::on_btnWorkExperienceReport_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnWorkExperienceReport);

    if (!workExperienceReportWindow) {
        workExperienceReportWindow = new WorkExperienceReportWindow(this);
    }
    workExperienceReportWindow->show();
    this->hide();
}

void ReportSelectionWindow::on_btnEducationReport_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnEducationReport);

    if (!educationReportWindow) {
        educationReportWindow = new EducationReportWindow(this);
    }
    educationReportWindow->show();
    this->hide();
}

void ReportSelectionWindow::on_btnPersonnelReport_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnPersonnelReport);

    if (!personnelReportWindow) {
        personnelReportWindow = new PersonnelReportWindow(this);
    }
    personnelReportWindow->show();
    this->hide();
}

void ReportSelectionWindow::on_btnDepartmentReport_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnDepartmentReport);

    if (!departmentReportWindow) {
        departmentReportWindow = new DepartmentReportWindow(this);
    }
    departmentReportWindow->show();
    this->hide();
}

void ReportSelectionWindow::on_btnPositionReport_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnPositionReport);

    if (!positionReportWindow) {
        positionReportWindow = new PositionReportWindow(this);
    }
    positionReportWindow->show();
    this->hide();
}

void ReportSelectionWindow::on_btnAssignmentReport_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnAssignmentReport);

    if (!assignmentReportWindow) {
        assignmentReportWindow = new AssignmentReportWindow(this);
    }
    assignmentReportWindow->show();
    this->hide();
}


void ReportSelectionWindow::on_btnBack_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnBack);

    if (parent()) {
        QWidget* parentWidget = qobject_cast<QWidget*>(parent());
        if (parentWidget) {
            parentWidget->show();
        }
    }
    this->hide();
}
