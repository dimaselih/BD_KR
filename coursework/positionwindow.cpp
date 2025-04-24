#include "positionwindow.h"
#include "ui_positionwindow.h"
#include <QSignalBlocker>
#include "databasemanager.h"

PositionWindow::PositionWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PositionWindow)
{
    ui->setupUi(this);

    // Настройка таблицы
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID" << "Название должности" << "Зарплата");
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
    connect(ui->btnAdd, &QPushButton::clicked, this, &PositionWindow::on_btnAdd_clicked);
    connect(ui->btnEdit, &QPushButton::clicked, this, &PositionWindow::on_btnEdit_clicked);
    connect(ui->btnDelete, &QPushButton::clicked, this, &PositionWindow::on_btnDelete_clicked);
    connect(ui->btnBack, &QPushButton::clicked, this, &PositionWindow::on_btnBack_clicked);
    connect(ui->tableWidget, &QTableWidget::itemClicked, this, &PositionWindow::on_tableWidget_itemClicked);

    // Загрузка данных
    loadData();
}

PositionWindow::~PositionWindow()
{
    delete ui;
}

void PositionWindow::loadData()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

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

    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("position_id").toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("position_name").toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("salary").toString()));

        row++;
    }

    ui->teResult->append(QString("Loaded %1 records.").arg(row));

    // Автоматическое выравнивание столбцов
    ui->tableWidget->resizeColumnsToContents();

    // Обновляем таблицу
    ui->tableWidget->update();
}

void PositionWindow::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    int row = item->row();

    ui->lePositionName->setText(ui->tableWidget->item(row, 1)->text());
    ui->dsbSalary->setValue(ui->tableWidget->item(row, 2)->text().toDouble());
}

void PositionWindow::on_btnAdd_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnAdd);

    if (!DatabaseManager::instance().isConnected() && !DatabaseManager::instance().connectToDatabase()) {
        ui->teResult->append("Error: No connection to the database.");
        return;
    }

    QSqlQuery query(DatabaseManager::instance().database());
    QString sqlstr = "INSERT INTO Positions (position_name, salary) VALUES (?, ?)";
    query.prepare(sqlstr);

    query.bindValue(0, ui->lePositionName->text());
    query.bindValue(1, ui->dsbSalary->value());

    if (!query.exec()) {
        ui->teResult->append("Error: " + query.lastError().text());
        return;
    }

    ui->teResult->append("Record added successfully.");
    loadData();
}

void PositionWindow::on_btnEdit_clicked()
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
    QString sqlstr = "UPDATE Positions SET position_name = ?, salary = ? WHERE position_id = ?";
    query.prepare(sqlstr);

    query.bindValue(0, ui->lePositionName->text());
    query.bindValue(1, ui->dsbSalary->value());
    query.bindValue(2, ui->tableWidget->item(currow, 0)->text());

    if (!query.exec()) {
        ui->teResult->append("Error: " + query.lastError().text());
        return;
    }

    ui->teResult->append("Record updated successfully.");
    loadData();
}

void PositionWindow::on_btnDelete_clicked()
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

    // Проверяем, используется ли должность в назначениях
    QSqlQuery checkQuery(DatabaseManager::instance().database());
    checkQuery.prepare("SELECT COUNT(*) FROM Assignments WHERE position_id = ?");
    checkQuery.bindValue(0, ui->tableWidget->item(currow, 0)->text());

    if (!checkQuery.exec() || !checkQuery.next()) {
        ui->teResult->append("Error checking dependencies: " + checkQuery.lastError().text());
        return;
    }

    int count = checkQuery.value(0).toInt();
    if (count > 0) {
        ui->teResult->append("Cannot delete position: There are " + QString::number(count) +
                            " assignments using this position. Remove these assignments first.");
        return;
    }

    QSqlQuery query(DatabaseManager::instance().database());
    QString sqlstr = "DELETE FROM Positions WHERE position_id = ?";
    query.prepare(sqlstr);
    query.bindValue(0, ui->tableWidget->item(currow, 0)->text());

    if (!query.exec()) {
        ui->teResult->append("Error: " + query.lastError().text());
        return;
    }

    ui->teResult->append("Record deleted successfully.");
    loadData();
}

void PositionWindow::on_btnBack_clicked()
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
