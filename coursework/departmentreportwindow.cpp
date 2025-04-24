#include "departmentreportwindow.h"
#include "ui_departmentreportwindow.h"
#include <QSignalBlocker>
#include <QWidget>
#include "databasemanager.h"

DepartmentReportWindow::DepartmentReportWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DepartmentReportWindow)
{
    ui->setupUi(this);

    // Настройка таблицы сотрудников
    ui->tableEmployees->setColumnCount(4);
    ui->tableEmployees->setHorizontalHeaderLabels(QStringList() << "ID" << "ФИО" << "Должность" << "Дата назначения");
    ui->tableEmployees->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableEmployees->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableEmployees->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Отключаем все сигналы
    disconnect(ui->btnBack, nullptr, nullptr, nullptr);
    disconnect(ui->btnSearch, nullptr, nullptr, nullptr);

    // Подключаем сигналы только один раз
    connect(ui->btnBack, &QPushButton::clicked, this, &DepartmentReportWindow::on_btnBack_clicked);
    connect(ui->btnSearch, &QPushButton::clicked, this, &DepartmentReportWindow::on_btnSearch_clicked);

    // Подключаемся к базе данных и загружаем данные при создании формы
    loadDepartmentComboBox();
    loadData(); // Загружаем все данные при открытии формы
}

DepartmentReportWindow::~DepartmentReportWindow()
{
    delete ui;
}

void DepartmentReportWindow::loadDepartmentComboBox()
{
    if (!DatabaseManager::instance().isConnected() && !DatabaseManager::instance().connectToDatabase()) {
        return;
    }

    QSqlQuery query(DatabaseManager::instance().database());
    QString sqlstr = "SELECT department_id, department_name FROM Departments ORDER BY department_name";

    if (!query.exec(sqlstr)) {
        return;
    }

    ui->cbDepartment->clear();
    ui->cbDepartment->addItem("Все отделы", -1);

    while (query.next()) {
        QString id = query.value("department_id").toString();
        QString name = query.value("department_name").toString();
        ui->cbDepartment->addItem(name, id);
    }
}

void DepartmentReportWindow::showDepartmentManager(int departmentId)
{
    ui->lblManager->clear();

    if (!DatabaseManager::instance().isConnected() && !DatabaseManager::instance().connectToDatabase()) {
        return;
    }

    QSqlQuery query(DatabaseManager::instance().database());
    QString sqlstr;

    if (departmentId == -1) {
        sqlstr = "SELECT d.department_name, p.full_name, a.assignment_date "
                 "FROM Departments d "
                 "JOIN Assignments a ON d.department_id = a.department_id "
                 "JOIN Personnel p ON a.personnel_id = p.personnel_id "
                 "JOIN Positions pos ON a.position_id = pos.position_id "
                 "WHERE pos.position_name = 'Руководитель отдела' "
                 "ORDER BY d.department_name";

        if (!query.exec(sqlstr)) {
            return;
        }

        QString managerInfo;
        while (query.next()) {
            managerInfo += QString("Отдел: %1, Руководитель: %2 (с %3)\n")
                          .arg(query.value("department_name").toString())
                          .arg(query.value("full_name").toString())
                          .arg(query.value("assignment_date").toDate().toString("dd.MM.yyyy"));
        }
        ui->lblManager->setText(managerInfo);
    } else {
        sqlstr = "SELECT d.department_name, p.full_name, a.assignment_date "
                 "FROM Departments d "
                 "JOIN Assignments a ON d.department_id = a.department_id "
                 "JOIN Personnel p ON a.personnel_id = p.personnel_id "
                 "JOIN Positions pos ON a.position_id = pos.position_id "
                 "WHERE pos.position_name = 'Руководитель отдела' AND d.department_id = ? "
                 "ORDER BY a.assignment_date DESC LIMIT 1";

        query.prepare(sqlstr);
        query.bindValue(0, departmentId);

        if (!query.exec() || !query.next()) {
            ui->lblManager->setText("Руководитель не назначен");
            return;
        }

        QString managerInfo = QString("Руководитель: %1 (с %2)")
                            .arg(query.value("full_name").toString())
                            .arg(query.value("assignment_date").toDate().toString("dd.MM.yyyy"));
        ui->lblManager->setText(managerInfo);
    }
}

void DepartmentReportWindow::loadData(int departmentId)
{
    ui->tableEmployees->clearContents();
    ui->tableEmployees->setRowCount(0);

    if (!DatabaseManager::instance().isConnected() && !DatabaseManager::instance().connectToDatabase()) {
        return;
    }

    // Обновляем информацию о руководителе
    showDepartmentManager(departmentId);

    // Загружаем список сотрудников
    QSqlQuery query(DatabaseManager::instance().database());
    QString sqlstr;

    if (departmentId == -1) {
        sqlstr = "SELECT d.department_name, p.personnel_id, p.full_name, pos.position_name, a.assignment_date "
                 "FROM Assignments a "
                 "JOIN Personnel p ON a.personnel_id = p.personnel_id "
                 "JOIN Positions pos ON a.position_id = pos.position_id "
                 "JOIN Departments d ON a.department_id = d.department_id "
                 "WHERE pos.position_name != 'Руководитель отдела' "
                 "ORDER BY d.department_name, p.full_name";

        if (!query.exec(sqlstr)) {
            return;
        }
    } else {
        sqlstr = "SELECT p.personnel_id, p.full_name, pos.position_name, a.assignment_date "
                 "FROM Assignments a "
                 "JOIN Personnel p ON a.personnel_id = p.personnel_id "
                 "JOIN Positions pos ON a.position_id = pos.position_id "
                 "WHERE a.department_id = ? AND pos.position_name != 'Руководитель отдела' "
                 "ORDER BY p.full_name";

        query.prepare(sqlstr);
        query.bindValue(0, departmentId);

        if (!query.exec()) {
            return;
        }
    }

    int row = 0;
    while (query.next()) {
        ui->tableEmployees->insertRow(row);

        ui->tableEmployees->setItem(row, 0, new QTableWidgetItem(query.value("personnel_id").toString()));
        ui->tableEmployees->setItem(row, 1, new QTableWidgetItem(query.value("full_name").toString()));
        ui->tableEmployees->setItem(row, 2, new QTableWidgetItem(query.value("position_name").toString()));
        ui->tableEmployees->setItem(row, 3, new QTableWidgetItem(query.value("assignment_date").toString()));

        // Если отображаем все отделы, добавляем название отдела в всплывающую подсказку
        if (departmentId == -1) {
            ui->tableEmployees->item(row, 1)->setToolTip("Отдел: " + query.value("department_name").toString());
        }

        row++;
    }

    // Автоматическое выравнивание столбцов
    ui->tableEmployees->resizeColumnsToContents();

    // Обновляем таблицу
    ui->tableEmployees->update();
}

void DepartmentReportWindow::on_btnSearch_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnSearch);

    int departmentId = ui->cbDepartment->currentData().toInt();
    loadData(departmentId);
}

void DepartmentReportWindow::on_btnBack_clicked()
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
