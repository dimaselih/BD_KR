#include "workexperiencewindow.h"
#include "ui_workexperiencewindow.h"
#include <QSignalBlocker>
#include "databasemanager.h"

WorkExperienceWindow::WorkExperienceWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WorkExperienceWindow)
{
    ui->setupUi(this);

    // Настройка таблицы
    ui->tableWidget->setColumnCount(6); // Устанавливаем количество столбцов (с ID)
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID" << "Personnel ID" << "Company Name" << "Position" << "Start Date" << "End Date");
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
    connect(ui->btnAdd, &QPushButton::clicked, this, &WorkExperienceWindow::on_btnAdd_clicked);
    connect(ui->btnEdit, &QPushButton::clicked, this, &WorkExperienceWindow::on_btnEdit_clicked);
    connect(ui->btnDelete, &QPushButton::clicked, this, &WorkExperienceWindow::on_btnDelete_clicked);
    connect(ui->btnBack, &QPushButton::clicked, this, &WorkExperienceWindow::on_btnBack_clicked);
    connect(ui->tableWidget, &QTableWidget::itemClicked, this, &WorkExperienceWindow::on_tableWidget_itemClicked);

    // Подключаемся к базе данных и загружаем данные при создании формы
    loadPersonnelComboBox();
    loadData();
}

WorkExperienceWindow::~WorkExperienceWindow()
{
    delete ui;
}

void WorkExperienceWindow::loadPersonnelComboBox()
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

    ui->cbPersonnelId->clear();

    while (query.next()) {
        QString id = query.value("personnel_id").toString();
        QString name = query.value("full_name").toString();
        ui->cbPersonnelId->addItem(name, id); // Добавляем имя как текст, а ID как данные
    }
}

void WorkExperienceWindow::loadData()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    if (!DatabaseManager::instance().isConnected() && !DatabaseManager::instance().connectToDatabase()) {
        ui->teResult->append("Error: No connection to the database.");
        return;
    }

    QSqlQuery query(DatabaseManager::instance().database());
    QString sqlstr = "SELECT w.work_experience_id, w.personnel_id, p.full_name, w.company_name, w.position, w.start_date, w.end_date FROM WorkExperience w JOIN Personnel p ON w.personnel_id = p.personnel_id";

    if (!query.exec(sqlstr)) {
        ui->teResult->append("Error: " + query.lastError().text());
        return;
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

        // Сохраняем personnel_id в пользовательских данных
        QTableWidgetItem* item = ui->tableWidget->item(row, 1);
        if (item) {
            item->setData(Qt::UserRole, query.value("personnel_id"));
        }

        row++;
    }

    ui->teResult->append(QString("Loaded %1 records.").arg(row));

    // Автоматическое выравнивание столбцов
    ui->tableWidget->resizeColumnsToContents();

    // Обновляем таблицу
    ui->tableWidget->update();
}

void WorkExperienceWindow::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    int row = item->row();

    // Получаем personnel_id из пользовательских данных
    QTableWidgetItem* personnelItem = ui->tableWidget->item(row, 1);
    if (personnelItem) {
        QVariant personnelId = personnelItem->data(Qt::UserRole);
        int index = ui->cbPersonnelId->findData(personnelId);
        if (index != -1) {
            ui->cbPersonnelId->setCurrentIndex(index);
        }
    }

    ui->leCompanyName->setText(ui->tableWidget->item(row, 2)->text());
    ui->lePosition->setText(ui->tableWidget->item(row, 3)->text());

    // Устанавливаем даты
    QDate startDate = QDate::fromString(ui->tableWidget->item(row, 4)->text(), "yyyy-MM-dd");
    QDate endDate = QDate::fromString(ui->tableWidget->item(row, 5)->text(), "yyyy-MM-dd");

    ui->deStartDate->setDate(startDate);
    ui->deEndDate->setDate(endDate);
}

void WorkExperienceWindow::on_btnAdd_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnAdd);

    if (!DatabaseManager::instance().isConnected() && !DatabaseManager::instance().connectToDatabase()) {
        ui->teResult->append("Error: No connection to the database.");
        return;
    }

    QSqlQuery query(DatabaseManager::instance().database());
    QString sqlstr = "INSERT INTO WorkExperience (personnel_id, company_name, position, start_date, end_date) VALUES (?, ?, ?, ?, ?)";
    query.prepare(sqlstr);

    // Получаем personnel_id из выбранного элемента в комбобоксе
    int personnelId = ui->cbPersonnelId->currentData().toInt();

    query.bindValue(0, personnelId);
    query.bindValue(1, ui->leCompanyName->text());
    query.bindValue(2, ui->lePosition->text());
    query.bindValue(3, ui->deStartDate->date().toString("yyyy-MM-dd"));
    query.bindValue(4, ui->deEndDate->date().toString("yyyy-MM-dd"));

    if (!query.exec()) {
        ui->teResult->append("Error: " + query.lastError().text());
        return;
    }

    ui->teResult->append("Record added successfully.");
    loadData();
}

void WorkExperienceWindow::on_btnEdit_clicked()
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
    QString sqlstr = "UPDATE WorkExperience SET personnel_id = ?, company_name = ?, position = ?, start_date = ?, end_date = ? WHERE work_experience_id = ?";
    query.prepare(sqlstr);

    // Получаем personnel_id из выбранного элемента в комбобоксе
    int personnelId = ui->cbPersonnelId->currentData().toInt();

    query.bindValue(0, personnelId);
    query.bindValue(1, ui->leCompanyName->text());
    query.bindValue(2, ui->lePosition->text());
    query.bindValue(3, ui->deStartDate->date().toString("yyyy-MM-dd"));
    query.bindValue(4, ui->deEndDate->date().toString("yyyy-MM-dd"));
    query.bindValue(5, ui->tableWidget->item(currow, 0)->text());

    if (!query.exec()) {
        ui->teResult->append("Error: " + query.lastError().text());
        return;
    }

    ui->teResult->append("Record updated successfully.");
    loadData();
}

void WorkExperienceWindow::on_btnDelete_clicked()
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
    QString sqlstr = "DELETE FROM WorkExperience WHERE work_experience_id = ?";
    query.prepare(sqlstr);
    query.bindValue(0, ui->tableWidget->item(currow, 0)->text());

    if (!query.exec()) {
        ui->teResult->append("Error: " + query.lastError().text());
        return;
    }

    ui->teResult->append("Record deleted successfully.");
    loadData();
}

void WorkExperienceWindow::on_btnBack_clicked()
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
