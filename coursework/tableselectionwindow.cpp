#include "tableselectionwindow.h"
#include "ui_tableselectionwindow.h"
#include <QSignalBlocker>
#include <QWidget>

TableSelectionWindow::TableSelectionWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TableSelectionWindow),
    workExperienceWindow(nullptr),
    educationWindow(nullptr),
    personnelWindow(nullptr),
    departmentWindow(nullptr),
    positionWindow(nullptr),
    assignmentWindow(nullptr)
{
    ui->setupUi(this);

    // Подключаем сигналы к слотам
    connect(ui->btnWorkExperience, &QPushButton::clicked, this, &TableSelectionWindow::on_btnWorkExperience_clicked);
    connect(ui->btnEducation, &QPushButton::clicked, this, &TableSelectionWindow::on_btnEducation_clicked);
    connect(ui->btnPersonnel, &QPushButton::clicked, this, &TableSelectionWindow::on_btnPersonnel_clicked);
    connect(ui->btnDepartment, &QPushButton::clicked, this, &TableSelectionWindow::on_btnDepartment_clicked);
    connect(ui->btnPosition, &QPushButton::clicked, this, &TableSelectionWindow::on_btnPosition_clicked);
    connect(ui->btnAssignment, &QPushButton::clicked, this, &TableSelectionWindow::on_btnAssignment_clicked);
    connect(ui->btnBack, &QPushButton::clicked, this, &TableSelectionWindow::on_btnBack_clicked);
}



TableSelectionWindow::~TableSelectionWindow()
{
    delete ui;
    if (workExperienceWindow)
        delete workExperienceWindow;
    if (educationWindow)
        delete educationWindow;
    if (personnelWindow)
        delete personnelWindow;
    if (departmentWindow)
        delete departmentWindow;
    if (positionWindow)
        delete positionWindow;
    if (assignmentWindow)
        delete assignmentWindow;
}



void TableSelectionWindow::on_btnWorkExperience_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnWorkExperience);

    if (!workExperienceWindow) {
        workExperienceWindow = new WorkExperienceWindow(this);
    }
    workExperienceWindow->show();
    this->hide();
}

void TableSelectionWindow::on_btnEducation_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnEducation);

    if (!educationWindow) {
        educationWindow = new EducationWindow(this);
    }
    educationWindow->show();
    this->hide();
}

void TableSelectionWindow::on_btnPersonnel_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnPersonnel);

    if (!personnelWindow) {
        personnelWindow = new PersonnelWindow(this);
    }
    personnelWindow->show();
    this->hide();
}

void TableSelectionWindow::on_btnDepartment_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnDepartment);

    if (!departmentWindow) {
        departmentWindow = new DepartmentWindow(this);
    }
    departmentWindow->show();
    this->hide();
}

void TableSelectionWindow::on_btnPosition_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnPosition);

    if (!positionWindow) {
        positionWindow = new PositionWindow(this);
    }
    positionWindow->show();
    this->hide();
}

void TableSelectionWindow::on_btnAssignment_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnAssignment);

    if (!assignmentWindow) {
        assignmentWindow = new AssignmentWindow(this);
    }
    assignmentWindow->show();
    this->hide();
}


void TableSelectionWindow::on_btnBack_clicked()
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
