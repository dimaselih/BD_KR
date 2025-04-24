#include "positionreportwindow.h"
#include "ui_positionreportwindow.h"
#include <QSignalBlocker>
#include <QWidget>
#include "databasemanager.h"

PositionReportWindow::PositionReportWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PositionReportWindow)
{
    ui->setupUi(this);

    // Настройка таблицы
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID" << "ФИО" << "Отдел" << "Дата назначения" << "Зарплата");
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Отключаем все сигналы
    disconnect(ui->btnBack, nullptr, nullptr, nullptr);
    disconnect(ui->btnSearch, nullptr, nullptr, nullptr);

    // Подключаем сигналы только один раз
    connect(ui->btnBack, &QPushButton::clicked, this, &PositionReportWindow::on_btnBack_clicked);
    connect(ui->btnSearch, &QPushButton::clicked, this, &PositionReportWindow::on_btnSearch_clicked);

    // Подключаемся к базе данных и загружаем данные при создании формы
    loadPositionComboBox();
    loadData(); // Загружаем все данные при открытии формы
}

PositionReportWindow::~PositionReportWindow()
{
    delete ui;
}

void PositionReportWindow::loadPositionComboBox()
{
    if (!DatabaseManager::instance().isConnected() && !DatabaseManager::instance().connectToDatabase()) {
        return;
    }

    QSqlQuery query(DatabaseManager::instance().database());
    QString sqlstr = "SELECT position_id, position_name FROM Positions ORDER BY position_name";

    if (!query.exec(sqlstr)) {
        return;
    }

    ui->cbPosition->clear();
    ui->cbPosition->addItem("Все должности", -1);

    while (query.next()) {
        QString id = query.value("position_id").toString();
        QString name = query.value("position_name").toString();
        ui->cbPosition->addItem(name, id);
    }
}

void PositionReportWindow::loadData(int positionId)
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    if (!DatabaseManager::instance().isConnected() && !DatabaseManager::instance().connectToDatabase()) {
        return;
    }

    // Обновляем информацию о должности
    if (positionId != -1) {
        QSqlQuery posQuery(DatabaseManager::instance().database());
        posQuery.prepare("SELECT position_name, salary FROM Positions WHERE position_id = ?");
        posQuery.bindValue(0, positionId);

        if (posQuery.exec() && posQuery.next()) {
            QString positionInfo = QString("Должность: %1, Зарплата: %2")
                                  .arg(posQuery.value("position_name").toString())
                                  .arg(posQuery.value("salary").toString());
            ui->lblPositionInfo->setText(positionInfo);
        } else {
            ui->lblPositionInfo->setText("Информация о должности не найдена");
        }
    } else {
        ui->lblPositionInfo->setText("Выберите должность для просмотра подробной информации");
    }

    // Загружаем список сотрудников
    QSqlQuery query(DatabaseManager::instance().database());
    QString sqlstr;

    if (positionId == -1) {
        sqlstr = "SELECT p.personnel_id, p.full_name, d.department_name, a.assignment_date, pos.salary "
                 "FROM Assignments a "
                 "JOIN Personnel p ON a.personnel_id = p.personnel_id "
                 "JOIN Positions pos ON a.position_id = pos.position_id "
                 "JOIN Departments d ON a.department_id = d.department_id "
                 "ORDER BY pos.position_name, p.full_name";

        if (!query.exec(sqlstr)) {
            return;
        }
    } else {
        sqlstr = "SELECT p.personnel_id, p.full_name, d.department_name, a.assignment_date, pos.salary "
                 "FROM Assignments a "
                 "JOIN Personnel p ON a.personnel_id = p.personnel_id "
                 "JOIN Positions pos ON a.position_id = pos.position_id "
                 "JOIN Departments d ON a.department_id = d.department_id "
                 "WHERE a.position_id = ? "
                 "ORDER BY p.full_name";

        query.prepare(sqlstr);
        query.bindValue(0, positionId);

        if (!query.exec()) {
            return;
        }
    }

    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("personnel_id").toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("full_name").toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("department_name").toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value("assignment_date").toDate().toString("dd.MM.yyyy")));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value("salary").toString()));

        row++;
    }

    // Обновляем статистику
    if (row > 0) {
        QString stats;
        if (positionId == -1) {
            stats = QString("Всего сотрудников: %1").arg(row);
        } else {
            stats = QString("Сотрудников на этой должности: %1").arg(row);
        }
        ui->lblStats->setText(stats);
    } else {
        ui->lblStats->setText("Нет сотрудников на этой должности");
    }

    // Автоматическое выравнивание столбцов
    ui->tableWidget->resizeColumnsToContents();

    // Обновляем таблицу
    ui->tableWidget->update();
}

void PositionReportWindow::on_btnSearch_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnSearch);

    int positionId = ui->cbPosition->currentData().toInt();
    loadData(positionId);
}

void PositionReportWindow::on_btnBack_clicked()
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
