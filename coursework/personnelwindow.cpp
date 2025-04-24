#include "personnelwindow.h"
#include "ui_personnelwindow.h"
#include <QSignalBlocker>
#include "databasemanager.h"

PersonnelWindow::PersonnelWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PersonnelWindow)
{
    ui->setupUi(this);

    // Настройка таблицы
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID" << "ФИО" << "Пол" << "Дата рождения" << "Специальность");
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
    connect(ui->btnAdd, &QPushButton::clicked, this, &PersonnelWindow::on_btnAdd_clicked);
    connect(ui->btnEdit, &QPushButton::clicked, this, &PersonnelWindow::on_btnEdit_clicked);
    connect(ui->btnDelete, &QPushButton::clicked, this, &PersonnelWindow::on_btnDelete_clicked);
    connect(ui->btnBack, &QPushButton::clicked, this, &PersonnelWindow::on_btnBack_clicked);
    connect(ui->tableWidget, &QTableWidget::itemClicked, this, &PersonnelWindow::on_tableWidget_itemClicked);

    // Настройка комбобокса для пола
    setupGenderComboBox();

    // Загрузка данных
    loadData();
}

PersonnelWindow::~PersonnelWindow()
{
    delete ui;
}

void PersonnelWindow::setupGenderComboBox()
{
    ui->cbGender->clear();
    ui->cbGender->addItem("М");
    ui->cbGender->addItem("Ж");
}

void PersonnelWindow::loadData()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    if (!DatabaseManager::instance().isConnected() && !DatabaseManager::instance().connectToDatabase()) {
        ui->teResult->append("Error: No connection to the database.");
        return;
    }

    QSqlQuery query(DatabaseManager::instance().database());
    QString sqlstr = "SELECT personnel_id, full_name, gender, birth_date, specialty FROM Personnel ORDER BY full_name";

    if (!query.exec(sqlstr)) {
        ui->teResult->append("Error: " + query.lastError().text());
        return;
    }

    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("personnel_id").toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("full_name").toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("gender").toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value("birth_date").toString()));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value("specialty").toString()));

        row++;
    }

    ui->teResult->append(QString("Loaded %1 records.").arg(row));

    // Автоматическое выравнивание столбцов
    ui->tableWidget->resizeColumnsToContents();

    // Обновляем таблицу
    ui->tableWidget->update();
}

void PersonnelWindow::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    int row = item->row();

    ui->leFullName->setText(ui->tableWidget->item(row, 1)->text());

    // Установка пола
    QString gender = ui->tableWidget->item(row, 2)->text();
    int genderIndex = ui->cbGender->findText(gender);
    if (genderIndex != -1) {
        ui->cbGender->setCurrentIndex(genderIndex);
    }

    // Установка даты рождения
    QDate birthDate = QDate::fromString(ui->tableWidget->item(row, 3)->text(), "yyyy-MM-dd");
    ui->deBirthDate->setDate(birthDate);

    ui->leSpecialty->setText(ui->tableWidget->item(row, 4)->text());
}

void PersonnelWindow::on_btnAdd_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnAdd);

    if (!DatabaseManager::instance().isConnected() && !DatabaseManager::instance().connectToDatabase()) {
        ui->teResult->append("Error: No connection to the database.");
        return;
    }

    QSqlQuery query(DatabaseManager::instance().database());
    QString sqlstr = "INSERT INTO Personnel (full_name, gender, birth_date, specialty) VALUES (?, ?, ?, ?)";
    query.prepare(sqlstr);

    query.bindValue(0, ui->leFullName->text());
    query.bindValue(1, ui->cbGender->currentText());
    query.bindValue(2, ui->deBirthDate->date().toString("yyyy-MM-dd"));
    query.bindValue(3, ui->leSpecialty->text());

    if (!query.exec()) {
        ui->teResult->append("Error: " + query.lastError().text());
        return;
    }

    ui->teResult->append("Record added successfully.");
    loadData();
}

void PersonnelWindow::on_btnEdit_clicked()
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
    QString sqlstr = "UPDATE Personnel SET full_name = ?, gender = ?, birth_date = ?, specialty = ? WHERE personnel_id = ?";
    query.prepare(sqlstr);

    query.bindValue(0, ui->leFullName->text());
    query.bindValue(1, ui->cbGender->currentText());
    query.bindValue(2, ui->deBirthDate->date().toString("yyyy-MM-dd"));
    query.bindValue(3, ui->leSpecialty->text());
    query.bindValue(4, ui->tableWidget->item(currow, 0)->text());

    if (!query.exec()) {
        ui->teResult->append("Error: " + query.lastError().text());
        return;
    }

    ui->teResult->append("Record updated successfully.");
    loadData();
}

void PersonnelWindow::on_btnDelete_clicked()
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
    QString sqlstr = "DELETE FROM Personnel WHERE personnel_id = ?";
    query.prepare(sqlstr);
    query.bindValue(0, ui->tableWidget->item(currow, 0)->text());

    if (!query.exec()) {
        ui->teResult->append("Error: " + query.lastError().text());
        return;
    }

    ui->teResult->append("Record deleted successfully.");
    loadData();
}

void PersonnelWindow::on_btnBack_clicked()
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
