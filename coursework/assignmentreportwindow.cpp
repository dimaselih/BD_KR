#include "assignmentreportwindow.h"
#include "ui_assignmentreportwindow.h"
#include <QSignalBlocker>
#include <QWidget>
#include "databasemanager.h"

AssignmentReportWindow::AssignmentReportWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AssignmentReportWindow)
{
    ui->setupUi(this);

    // Настройка таблицы
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID" << "Сотрудник" << "Отдел" << "Должность" << "Дата назначения" << "Зарплата");
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Настройка типов фильтров
    ui->cbFilterType->addItem("Все назначения", 0);
    ui->cbFilterType->addItem("По сотруднику", 1);
    ui->cbFilterType->addItem("По отделу", 2);
    ui->cbFilterType->addItem("По должности", 3);
    ui->cbFilterType->addItem("По периоду", 4);

    // Настройка дат
    ui->deStartDate->setDate(QDate::currentDate().addYears(-1));
    ui->deEndDate->setDate(QDate::currentDate());
    ui->deStartDate->setCalendarPopup(true);
    ui->deEndDate->setCalendarPopup(true);

    // Отключаем все сигналы
    disconnect(ui->btnBack, nullptr, nullptr, nullptr);
    disconnect(ui->btnSearch, nullptr, nullptr, nullptr);
    disconnect(ui->cbFilterType, nullptr, nullptr, nullptr);

    // Подключаем сигналы только один раз
    connect(ui->btnBack, &QPushButton::clicked, this, &AssignmentReportWindow::on_btnBack_clicked);
    connect(ui->btnSearch, &QPushButton::clicked, this, &AssignmentReportWindow::on_btnSearch_clicked);
    connect(ui->cbFilterType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AssignmentReportWindow::on_cbFilterType_currentIndexChanged);

    // Загружаем данные для комбобоксов и таблицы
    loadFilterComboBoxes();
    updateFilterVisibility(0); // По умолчанию показываем все назначения
    loadData();
}

AssignmentReportWindow::~AssignmentReportWindow()
{
    delete ui;
}

void AssignmentReportWindow::loadFilterComboBoxes()
{
    if (!DatabaseManager::instance().isConnected() && !DatabaseManager::instance().connectToDatabase()) {
        return;
    }

    // Загружаем список сотрудников
    QSqlQuery personnelQuery(DatabaseManager::instance().database());
    QString personnelSql = "SELECT personnel_id, full_name FROM Personnel ORDER BY full_name";

    if (personnelQuery.exec(personnelSql)) {
        ui->cbPersonnel->clear();
        while (personnelQuery.next()) {
            QString id = personnelQuery.value("personnel_id").toString();
            QString name = personnelQuery.value("full_name").toString();
            ui->cbPersonnel->addItem(name, id);
        }
    }

    // Загружаем список отделов
    QSqlQuery departmentQuery(DatabaseManager::instance().database());
    QString departmentSql = "SELECT department_id, department_name FROM Departments ORDER BY department_name";

    if (departmentQuery.exec(departmentSql)) {
        ui->cbDepartment->clear();
        while (departmentQuery.next()) {
            QString id = departmentQuery.value("department_id").toString();
            QString name = departmentQuery.value("department_name").toString();
            ui->cbDepartment->addItem(name, id);
        }
    }

    // Загружаем список должностей
    QSqlQuery positionQuery(DatabaseManager::instance().database());
    QString positionSql = "SELECT position_id, position_name FROM Positions ORDER BY position_name";

    if (positionQuery.exec(positionSql)) {
        ui->cbPosition->clear();
        while (positionQuery.next()) {
            QString id = positionQuery.value("position_id").toString();
            QString name = positionQuery.value("position_name").toString();
            ui->cbPosition->addItem(name, id);
        }
    }
}

void AssignmentReportWindow::updateFilterVisibility(int filterType)
{
    // Скрываем все фильтры
    ui->lblPersonnel->setVisible(false);
    ui->cbPersonnel->setVisible(false);
    ui->lblDepartment->setVisible(false);
    ui->cbDepartment->setVisible(false);
    ui->lblPosition->setVisible(false);
    ui->cbPosition->setVisible(false);
    ui->lblDateRange->setVisible(false);
    ui->deStartDate->setVisible(false);
    ui->lblTo->setVisible(false);
    ui->deEndDate->setVisible(false);

    // Показываем нужный фильтр в зависимости от выбранного типа
    switch (filterType) {
        case 1: // По сотруднику
            ui->lblPersonnel->setVisible(true);
            ui->cbPersonnel->setVisible(true);
            break;
        case 2: // По отделу
            ui->lblDepartment->setVisible(true);
            ui->cbDepartment->setVisible(true);
            break;
        case 3: // По должности
            ui->lblPosition->setVisible(true);
            ui->cbPosition->setVisible(true);
            break;
        case 4: // По периоду
            ui->lblDateRange->setVisible(true);
            ui->deStartDate->setVisible(true);
            ui->lblTo->setVisible(true);
            ui->deEndDate->setVisible(true);
            break;
        default: // Все назначения
            break;
    }
}

void AssignmentReportWindow::loadData()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    if (!DatabaseManager::instance().isConnected() && !DatabaseManager::instance().connectToDatabase()) {
        return;
    }

    QSqlQuery query(DatabaseManager::instance().database());
    QString sqlstr;
    int filterType = ui->cbFilterType->currentData().toInt();

    switch (filterType) {
        case 1: // По сотруднику
            sqlstr = "SELECT a.assignment_id, p.full_name, d.department_name, pos.position_name, "
                     "a.assignment_date, pos.salary "
                     "FROM Assignments a "
                     "JOIN Personnel p ON a.personnel_id = p.personnel_id "
                     "JOIN Departments d ON a.department_id = d.department_id "
                     "JOIN Positions pos ON a.position_id = pos.position_id "
                     "WHERE a.personnel_id = ? "
                     "ORDER BY a.assignment_date DESC";
            query.prepare(sqlstr);
            query.bindValue(0, ui->cbPersonnel->currentData().toInt());
            break;

        case 2: // По отделу
            sqlstr = "SELECT a.assignment_id, p.full_name, d.department_name, pos.position_name, "
                     "a.assignment_date, pos.salary "
                     "FROM Assignments a "
                     "JOIN Personnel p ON a.personnel_id = p.personnel_id "
                     "JOIN Departments d ON a.department_id = d.department_id "
                     "JOIN Positions pos ON a.position_id = pos.position_id "
                     "WHERE a.department_id = ? "
                     "ORDER BY pos.position_name, p.full_name";
            query.prepare(sqlstr);
            query.bindValue(0, ui->cbDepartment->currentData().toInt());
            break;

        case 3: // По должности
            sqlstr = "SELECT a.assignment_id, p.full_name, d.department_name, pos.position_name, "
                     "a.assignment_date, pos.salary "
                     "FROM Assignments a "
                     "JOIN Personnel p ON a.personnel_id = p.personnel_id "
                     "JOIN Departments d ON a.department_id = d.department_id "
                     "JOIN Positions pos ON a.position_id = pos.position_id "
                     "WHERE a.position_id = ? "
                     "ORDER BY d.department_name, p.full_name";
            query.prepare(sqlstr);
            query.bindValue(0, ui->cbPosition->currentData().toInt());
            break;

        case 4: // По периоду
            sqlstr = "SELECT a.assignment_id, p.full_name, d.department_name, pos.position_name, "
                     "a.assignment_date, pos.salary "
                     "FROM Assignments a "
                     "JOIN Personnel p ON a.personnel_id = p.personnel_id "
                     "JOIN Departments d ON a.department_id = d.department_id "
                     "JOIN Positions pos ON a.position_id = pos.position_id "
                     "WHERE a.assignment_date BETWEEN ? AND ? "
                     "ORDER BY a.assignment_date DESC";
            query.prepare(sqlstr);
            query.bindValue(0, ui->deStartDate->date().toString("yyyy-MM-dd"));
            query.bindValue(1, ui->deEndDate->date().toString("yyyy-MM-dd"));
            break;

        default: // Все назначения
            sqlstr = "SELECT a.assignment_id, p.full_name, d.department_name, pos.position_name, "
                     "a.assignment_date, pos.salary "
                     "FROM Assignments a "
                     "JOIN Personnel p ON a.personnel_id = p.personnel_id "
                     "JOIN Departments d ON a.department_id = d.department_id "
                     "JOIN Positions pos ON a.position_id = pos.position_id "
                     "ORDER BY a.assignment_date DESC";
            break;
    }

    if (filterType == 0) {
        if (!query.exec(sqlstr)) {
            return;
        }
    } else {
        if (!query.exec()) {
            return;
        }
    }

    int row = 0;
    double totalSalary = 0.0;

    while (query.next()) {
        ui->tableWidget->insertRow(row);

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("assignment_id").toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("full_name").toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("department_name").toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value("position_name").toString()));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value("assignment_date").toDate().toString("dd.MM.yyyy")));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value("salary").toString()));

        totalSalary += query.value("salary").toDouble();
        row++;
    }

    // Обновляем статистику
    QString stats;
    if (row > 0) {
        stats = QString("Всего назначений: %1, Общая сумма зарплат: %2").arg(row).arg(totalSalary);
    } else {
        stats = "Назначения не найдены";
    }
    ui->lblStats->setText(stats);

    // Автоматическое выравнивание столбцов
    ui->tableWidget->resizeColumnsToContents();

    // Обновляем таблицу
    ui->tableWidget->update();
}

void AssignmentReportWindow::on_cbFilterType_currentIndexChanged(int index)
{
    int filterType = ui->cbFilterType->currentData().toInt();
    updateFilterVisibility(filterType);
}

void AssignmentReportWindow::on_btnSearch_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnSearch);

    loadData();
}

void AssignmentReportWindow::on_btnBack_clicked()
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
