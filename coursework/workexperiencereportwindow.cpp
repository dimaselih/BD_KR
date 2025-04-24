#include "workexperiencereportwindow.h"
#include "ui_workexperiencereportwindow.h"
#include <QSignalBlocker>
#include <QWidget>
#include "databasemanager.h"

WorkExperienceReportWindow::WorkExperienceReportWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WorkExperienceReportWindow)
{
    ui->setupUi(this);

    // Настройка таблицы
    ui->tableWidget->setColumnCount(6); // Устанавливаем количество столбцов
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID" << "Personnel" << "Company Name" << "Position" << "Start Date" << "End Date");
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Отключаем все сигналы
    disconnect(ui->btnBack, nullptr, nullptr, nullptr);
    disconnect(ui->btnSearch, nullptr, nullptr, nullptr);

    // Подключаем сигналы только один раз
    connect(ui->btnBack, &QPushButton::clicked, this, &WorkExperienceReportWindow::on_btnBack_clicked);
    connect(ui->btnSearch, &QPushButton::clicked, this, &WorkExperienceReportWindow::on_btnSearch_clicked);

    // Подключаемся к базе данных и загружаем данные при создании формы
    loadPersonnelComboBox();
    loadData(); // Загружаем все данные при открытии формы
}

WorkExperienceReportWindow::~WorkExperienceReportWindow()
{
    delete ui;
}

void WorkExperienceReportWindow::loadPersonnelComboBox()
{
    if (!DatabaseManager::instance().isConnected() && !DatabaseManager::instance().connectToDatabase()) {
        return;
    }

    QSqlQuery query(DatabaseManager::instance().database());
    QString sqlstr = "SELECT personnel_id, full_name FROM Personnel ORDER BY full_name";

    if (!query.exec(sqlstr)) {
        return;
    }

    ui->cbPersonnel->clear();
    ui->cbPersonnel->addItem("All Personnel", -1); // Добавляем опцию "Все сотрудники"

    while (query.next()) {
        QString id = query.value("personnel_id").toString();
        QString name = query.value("full_name").toString();
        ui->cbPersonnel->addItem(name, id); // Добавляем имя как текст, а ID как данные
    }
}

void WorkExperienceReportWindow::loadData(int personnelId)
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    if (!DatabaseManager::instance().isConnected() && !DatabaseManager::instance().connectToDatabase()) {
        return;
    }

    QSqlQuery query(DatabaseManager::instance().database());
    QString sqlstr;

    if (personnelId == -1) {
        // Загрузить все записи
        sqlstr = "SELECT w.work_experience_id, p.full_name, w.company_name, w.position, w.start_date, w.end_date FROM WorkExperience w JOIN Personnel p ON w.personnel_id = p.personnel_id ORDER BY p.full_name, w.start_date DESC";
        if (!query.exec(sqlstr)) {
            return;
        }
    } else {
        // Загрузить записи только для выбранного сотрудника
        sqlstr = "SELECT w.work_experience_id, p.full_name, w.company_name, w.position, w.start_date, w.end_date FROM WorkExperience w JOIN Personnel p ON w.personnel_id = p.personnel_id WHERE w.personnel_id = ? ORDER BY w.start_date DESC";
        query.prepare(sqlstr);
        query.bindValue(0, personnelId);
        if (!query.exec()) {
            return;
        }
    }

    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("work_experience_id").toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("full_name").toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("company_name").toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value("position").toString()));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value("start_date").toString()));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value("end_date").toString()));

        row++;
    }

    // Автоматическое выравнивание столбцов
    ui->tableWidget->resizeColumnsToContents();

    // Обновляем таблицу
    ui->tableWidget->update();
}

void WorkExperienceReportWindow::on_btnSearch_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnSearch);

    int personnelId = ui->cbPersonnel->currentData().toInt();
    loadData(personnelId);
}

void WorkExperienceReportWindow::on_btnBack_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnBack);

    if (parent()) {
        QWidget* parentWidget = qobject_cast<QWidget*>(parent());
        if (parentWidget) {
            parentWidget->show();  // Показываем родительскую форму
        }
    }
    this->hide();
}

