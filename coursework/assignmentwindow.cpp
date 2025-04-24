#include "assignmentwindow.h"
#include "ui_assignmentwindow.h"
#include <QSignalBlocker>
#include <QDate>
#include "databasemanager.h"

AssignmentWindow::AssignmentWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AssignmentWindow)
{
    ui->setupUi(this);

    // Настройка таблицы
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID" << "Сотрудник" << "Отдел" << "Должность" << "Дата назначения" << "Зарплата");
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
    connect(ui->btnAdd, &QPushButton::clicked, this, &AssignmentWindow::on_btnAdd_clicked);
    connect(ui->btnEdit, &QPushButton::clicked, this, &AssignmentWindow::on_btnEdit_clicked);
    connect(ui->btnDelete, &QPushButton::clicked, this, &AssignmentWindow::on_btnDelete_clicked);
    connect(ui->btnBack, &QPushButton::clicked, this, &AssignmentWindow::on_btnBack_clicked);
    connect(ui->tableWidget, &QTableWidget::itemClicked, this, &AssignmentWindow::on_tableWidget_itemClicked);

    // Настройка даты назначения
    ui->deAssignmentDate->setDate(QDate::currentDate());
    ui->deAssignmentDate->setCalendarPopup(true);

    // Загрузка данных для комбобоксов и таблицы
    loadPersonnelComboBox();
    loadDepartmentComboBox();
    loadPositionComboBox();
    loadData();
}

AssignmentWindow::~AssignmentWindow()
{
    delete ui;
}

void AssignmentWindow::loadPersonnelComboBox()
{
    if (!DatabaseManager::instance().isConnected() && !DatabaseManager::instance().connectToDatabase()) {
        ui->teResult->append("Error: No connection to the database.");
        return;
    }

    QSqlQuery query(DatabaseManager::instance().database());
    QString sqlstr = "SELECT personnel_id, full_name FROM Personnel ORDER BY full_name";

    if (!query.exec(sqlstr)) {
        ui->teResult->append("Error: " + query.lastError().text());
        return;
    }

    ui->cbPersonnel->clear();

    while (query.next()) {
        QString id = query.value("personnel_id").toString();
        QString name = query.value("full_name").toString();
        ui->cbPersonnel->addItem(name, id);
    }
}

void AssignmentWindow::loadDepartmentComboBox()
{
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

    ui->cbDepartment->clear();

    while (query.next()) {
        QString id = query.value("department_id").toString();
        QString name = query.value("department_name").toString();
        ui->cbDepartment->addItem(name, id);
    }
}

void AssignmentWindow::loadPositionComboBox()
{
    if (!DatabaseManager::instance().isConnected() && !DatabaseManager::instance().connectToDatabase()) {
        ui->teResult->append("Error: No connection to the database.");
        return;
    }

    QSqlQuery query(DatabaseManager::instance().database());
    QString sqlstr = "SELECT position_id, position_name, salary FROM Positions ORDER BY position_name";

    if (!query.exec(sqlstr)) {
        ui->teResult->append("Error: " + query.lastError().text());
        return;
    }

    ui->cbPosition->clear();

    while (query.next()) {
        QString id = query.value("position_id").toString();
        QString name = query.value("position_name").toString();
        double salary = query.value("salary").toDouble();
        QString displayText = QString("%1 (Зарплата: %2)").arg(name).arg(salary);
        ui->cbPosition->addItem(displayText, id);
    }
}

void AssignmentWindow::loadData()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    if (!DatabaseManager::instance().isConnected() && !DatabaseManager::instance().connectToDatabase()) {
        ui->teResult->append("Error: No connection to the database.");
        return;
    }

    QSqlQuery query(DatabaseManager::instance().database());
    QString sqlstr = "SELECT a.assignment_id, p.personnel_id, p.full_name, d.department_id, d.department_name, "
                     "pos.position_id, pos.position_name, a.assignment_date, pos.salary "
                     "FROM Assignments a "
                     "JOIN Personnel p ON a.personnel_id = p.personnel_id "
                     "JOIN Departments d ON a.department_id = d.department_id "
                     "JOIN Positions pos ON a.position_id = pos.position_id "
                     "ORDER BY a.assignment_date DESC";

    if (!query.exec(sqlstr)) {
        ui->teResult->append("Error: " + query.lastError().text());
        return;
    }

    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("assignment_id").toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("full_name").toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("department_name").toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value("position_name").toString()));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value("assignment_date").toDate().toString("dd.MM.yyyy")));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value("salary").toString()));

        // Сохраняем ID в пользовательских данных
        QTableWidgetItem* personnelItem = ui->tableWidget->item(row, 1);
        if (personnelItem) {
            personnelItem->setData(Qt::UserRole, query.value("personnel_id"));
        }

        QTableWidgetItem* departmentItem = ui->tableWidget->item(row, 2);
        if (departmentItem) {
            departmentItem->setData(Qt::UserRole, query.value("department_id"));
        }

        QTableWidgetItem* positionItem = ui->tableWidget->item(row, 3);
        if (positionItem) {
            positionItem->setData(Qt::UserRole, query.value("position_id"));
        }

        row++;
    }

    ui->teResult->append(QString("Loaded %1 records.").arg(row));

    // Автоматическое выравнивание столбцов
    ui->tableWidget->resizeColumnsToContents();

    // Обновляем таблицу
    ui->tableWidget->update();
}

void AssignmentWindow::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    int row = item->row();

    // Получаем personnel_id из пользовательских данных и устанавливаем соответствующий элемент в комбобоксе
    QTableWidgetItem* personnelItem = ui->tableWidget->item(row, 1);
    if (personnelItem) {
        QVariant personnelId = personnelItem->data(Qt::UserRole);
        int index = ui->cbPersonnel->findData(personnelId);
        if (index != -1) {
            ui->cbPersonnel->setCurrentIndex(index);
        }
    }

    // Получаем department_id из пользовательских данных и устанавливаем соответствующий элемент в комбобоксе
    QTableWidgetItem* departmentItem = ui->tableWidget->item(row, 2);
    if (departmentItem) {
        QVariant departmentId = departmentItem->data(Qt::UserRole);
        int index = ui->cbDepartment->findData(departmentId);
        if (index != -1) {
            ui->cbDepartment->setCurrentIndex(index);
        }
    }

    // Получаем position_id из пользовательских данных и устанавливаем соответствующий элемент в комбобоксе
    QTableWidgetItem* positionItem = ui->tableWidget->item(row, 3);
    if (positionItem) {
        QVariant positionId = positionItem->data(Qt::UserRole);
        int index = ui->cbPosition->findData(positionId);
        if (index != -1) {
            ui->cbPosition->setCurrentIndex(index);
        }
    }

    // Устанавливаем дату назначения
    QString dateStr = ui->tableWidget->item(row, 4)->text();
    QDate assignmentDate = QDate::fromString(dateStr, "dd.MM.yyyy");
    if (!assignmentDate.isValid()) {
        // Пробуем другой формат, если первый не сработал
        assignmentDate = QDate::fromString(dateStr, "yyyy-MM-dd");
    }
    if (assignmentDate.isValid()) {
        ui->deAssignmentDate->setDate(assignmentDate);
    }
}

void AssignmentWindow::on_btnAdd_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnAdd);

    if (!DatabaseManager::instance().isConnected() && !DatabaseManager::instance().connectToDatabase()) {
        ui->teResult->append("Error: No connection to the database.");
        return;
    }

    QSqlQuery query(DatabaseManager::instance().database());
    QString sqlstr = "INSERT INTO Assignments (personnel_id, department_id, position_id, assignment_date) VALUES (?, ?, ?, ?)";
    query.prepare(sqlstr);

    // Получаем ID из выбранных элементов в комбобоксах
    int personnelId = ui->cbPersonnel->currentData().toInt();
    int departmentId = ui->cbDepartment->currentData().toInt();
    int positionId = ui->cbPosition->currentData().toInt();

    query.bindValue(0, personnelId);
    query.bindValue(1, departmentId);
    query.bindValue(2, positionId);
    query.bindValue(3, ui->deAssignmentDate->date().toString("yyyy-MM-dd"));

    if (!query.exec()) {
        ui->teResult->append("Error: " + query.lastError().text());
        return;
    }

    ui->teResult->append("Record added successfully.");
    loadData();
}

void AssignmentWindow::on_btnEdit_clicked()
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
    QString sqlstr = "UPDATE Assignments SET personnel_id = ?, department_id = ?, position_id = ?, assignment_date = ? WHERE assignment_id = ?";
    query.prepare(sqlstr);

    // Получаем ID из выбранных элементов в комбобоксах
    int personnelId = ui->cbPersonnel->currentData().toInt();
    int departmentId = ui->cbDepartment->currentData().toInt();
    int positionId = ui->cbPosition->currentData().toInt();

    query.bindValue(0, personnelId);
    query.bindValue(1, departmentId);
    query.bindValue(2, positionId);
    query.bindValue(3, ui->deAssignmentDate->date().toString("yyyy-MM-dd"));
    query.bindValue(4, ui->tableWidget->item(currow, 0)->text());

    if (!query.exec()) {
        ui->teResult->append("Error: " + query.lastError().text());
        return;
    }

    ui->teResult->append("Record updated successfully.");
    loadData();
}

void AssignmentWindow::on_btnDelete_clicked()
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

    QSqlQuery query(DatabaseManager::instance().database());
    QString sqlstr = "DELETE FROM Assignments WHERE assignment_id = ?";
    query.prepare(sqlstr);
    query.bindValue(0, ui->tableWidget->item(currow, 0)->text());

    if (!query.exec()) {
        ui->teResult->append("Error: " + query.lastError().text());
        return;
    }

    ui->teResult->append("Record deleted successfully.");
    loadData();
}

void AssignmentWindow::on_btnBack_clicked()
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
