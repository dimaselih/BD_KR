#include "departmentwindow.h"
#include "ui_departmentwindow.h"
#include <QSignalBlocker>
#include "databasemanager.h"

DepartmentWindow::DepartmentWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DepartmentWindow)
{
    ui->setupUi(this);

    // Настройка таблицы
    ui->tableWidget->setColumnCount(2); // Теперь только ID и название
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID" << "Название отдела");
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Отключаем все сигналы
    disconnect(ui->btnAdd, nullptr, nullptr, nullptr);
    disconnect(ui->btnEdit, nullptr, nullptr, nullptr);
    disconnect(ui->btnDelete, nullptr, nullptr, nullptr);
    disconnect(ui->btnBack, nullptr, nullptr, nullptr);
    disconnect(ui->tableWidget, nullptr, nullptr, nullptr);

    // Подключаем сигналы только один раз
    connect(ui->btnAdd, &QPushButton::clicked, this, &DepartmentWindow::on_btnAdd_clicked);
    connect(ui->btnEdit, &QPushButton::clicked, this, &DepartmentWindow::on_btnEdit_clicked);
    connect(ui->btnDelete, &QPushButton::clicked, this, &DepartmentWindow::on_btnDelete_clicked);
    connect(ui->btnBack, &QPushButton::clicked, this, &DepartmentWindow::on_btnBack_clicked);
    connect(ui->tableWidget, &QTableWidget::itemClicked, this, &DepartmentWindow::on_tableWidget_itemClicked);

    // Загрузка данных
    loadData();
}

DepartmentWindow::~DepartmentWindow()
{
    delete ui;
}

void DepartmentWindow::loadData()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    if (!DatabaseManager::instance().isConnected() && !DatabaseManager::instance().connectToDatabase()) {
        ui->teResult->append("Error: No connection to the database.");
        return;
    }

    QSqlQuery query(DatabaseManager::instance().database());
    QString sqlstr = "SELECT department_id, department_name FROM Departments ORDER BY department_name";

    if (!query.exec(sqlstr)) {
        ui->teResult->append("Error: " + query.lastError().text());
        return;
    }

    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("department_id").toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("department_name").toString()));

        row++;
    }

    ui->teResult->append(QString("Loaded %1 records.").arg(row));

    // Автоматическое выравнивание столбцов
    ui->tableWidget->resizeColumnsToContents();

    // Обновляем таблицу
    ui->tableWidget->update();
}

void DepartmentWindow::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    int row = item->row();

    ui->leDepartmentName->setText(ui->tableWidget->item(row, 1)->text());
}

void DepartmentWindow::on_btnAdd_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnAdd);

    if (!DatabaseManager::instance().isConnected() && !DatabaseManager::instance().connectToDatabase()) {
        ui->teResult->append("Error: No connection to the database.");
        return;
    }

    QSqlQuery query(DatabaseManager::instance().database());
    QString sqlstr = "INSERT INTO Departments (department_name) VALUES (?)";
    query.prepare(sqlstr);

    query.bindValue(0, ui->leDepartmentName->text());

    if (!query.exec()) {
        ui->teResult->append("Error: " + query.lastError().text());
        return;
    }

    ui->teResult->append("Record added successfully.");
    loadData();
}

void DepartmentWindow::on_btnEdit_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnEdit);

    if (!DatabaseManager::instance().isConnected() && !DatabaseManager::instance().connectToDatabase()) {
        ui->teResult->append("Error: No connection to the database.");
        return;
    }

    int currow = ui->tableWidget->currentRow();
    if (currow < 0) {
        ui->teResult->append("Error: No row selected for editing.");
        return;
    }

    QSqlQuery query(DatabaseManager::instance().database());
    QString sqlstr = "UPDATE Departments SET department_name = ? WHERE department_id = ?";
    query.prepare(sqlstr);

    query.bindValue(0, ui->leDepartmentName->text());
    query.bindValue(1, ui->tableWidget->item(currow, 0)->text());

    if (!query.exec()) {
        ui->teResult->append("Error: " + query.lastError().text());
        return;
    }

    ui->teResult->append("Record updated successfully.");
    loadData();
}

void DepartmentWindow::on_btnDelete_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnDelete);

    if (!DatabaseManager::instance().isConnected() && !DatabaseManager::instance().connectToDatabase()) {
        ui->teResult->append("Error: No connection to the database.");
        return;
    }

    int currow = ui->tableWidget->currentRow();
    if (currow < 0) {
        ui->teResult->append("Error: No row selected for deletion.");
        return;
    }

    // Проверяем, есть ли назначения, связанные с этим отделом
    QSqlQuery checkQuery(DatabaseManager::instance().database());
    checkQuery.prepare("SELECT COUNT(*) FROM Assignments WHERE department_id = ?");
    checkQuery.bindValue(0, ui->tableWidget->item(currow, 0)->text());

    if (!checkQuery.exec() || !checkQuery.next()) {
        ui->teResult->append("Error checking dependencies: " + checkQuery.lastError().text());
        return;
    }

    int count = checkQuery.value(0).toInt();
    if (count > 0) {
        ui->teResult->append("Cannot delete department: There are " + QString::number(count) +
                            " assignments linked to this department. Remove these assignments first.");
        return;
    }

    QSqlQuery query(DatabaseManager::instance().database());
    QString sqlstr = "DELETE FROM Departments WHERE department_id = ?";
    query.prepare(sqlstr);
    query.bindValue(0, ui->tableWidget->item(currow, 0)->text());

    if (!query.exec()) {
        ui->teResult->append("Error: " + query.lastError().text());
        return;
    }

    ui->teResult->append("Record deleted successfully.");
    loadData();
}

void DepartmentWindow::on_btnBack_clicked()
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
