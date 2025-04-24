#include "educationwindow.h"
#include "ui_educationwindow.h"
#include <QSignalBlocker>
#include "databasemanager.h"

EducationWindow::EducationWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EducationWindow)
{
    ui->setupUi(this);

    // Настройка таблицы
    ui->tableWidget->setColumnCount(6); // Устанавливаем количество столбцов (с ID)
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID" << "Personnel" << "Institution Name" << "Degree" << "Specialty" << "Graduation Date");
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
    connect(ui->btnAdd, &QPushButton::clicked, this, &EducationWindow::on_btnAdd_clicked);
    connect(ui->btnEdit, &QPushButton::clicked, this, &EducationWindow::on_btnEdit_clicked);
    connect(ui->btnDelete, &QPushButton::clicked, this, &EducationWindow::on_btnDelete_clicked);
    connect(ui->btnBack, &QPushButton::clicked, this, &EducationWindow::on_btnBack_clicked);
    connect(ui->tableWidget, &QTableWidget::itemClicked, this, &EducationWindow::on_tableWidget_itemClicked);

    // Подключаемся к базе данных и загружаем данные при создании формы
    loadPersonnelComboBox();
    loadData();
}

EducationWindow::~EducationWindow()
{
    delete ui;
}

void EducationWindow::loadPersonnelComboBox()
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

void EducationWindow::loadData()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    if (!DatabaseManager::instance().isConnected() && !DatabaseManager::instance().connectToDatabase()) {
        ui->teResult->append("Error: No connection to the database.");
        return;
    }

    QSqlQuery query(DatabaseManager::instance().database());
    QString sqlstr = "SELECT e.education_id, e.personnel_id, p.full_name, e.institution_name, e.degree, e.specialty, e.graduation_date FROM Education e JOIN Personnel p ON e.personnel_id = p.personnel_id";

    if (!query.exec(sqlstr)) {
        ui->teResult->append("Error: " + query.lastError().text());
        return;
    }

    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("education_id").toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("full_name").toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("institution_name").toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value("degree").toString()));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value("specialty").toString()));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value("graduation_date").toString()));

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

void EducationWindow::on_tableWidget_itemClicked(QTableWidgetItem *item)
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

    ui->leInstitutionName->setText(ui->tableWidget->item(row, 2)->text());
    ui->leDegree->setText(ui->tableWidget->item(row, 3)->text());
    ui->leSpecialty->setText(ui->tableWidget->item(row, 4)->text());

    // Устанавливаем дату
    QDate graduationDate = QDate::fromString(ui->tableWidget->item(row, 5)->text(), "yyyy-MM-dd");
    ui->deGraduationDate->setDate(graduationDate);
}

void EducationWindow::on_btnAdd_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnAdd);

    if (!DatabaseManager::instance().isConnected() && !DatabaseManager::instance().connectToDatabase()) {
        ui->teResult->append("Error: No connection to the database.");
        return;
    }

    QSqlQuery query(DatabaseManager::instance().database());
    QString sqlstr = "INSERT INTO Education (personnel_id, institution_name, degree, specialty, graduation_date) VALUES (?, ?, ?, ?, ?)";
    query.prepare(sqlstr);

    // Получаем personnel_id из выбранного элемента в комбобоксе
    int personnelId = ui->cbPersonnelId->currentData().toInt();

    query.bindValue(0, personnelId);
    query.bindValue(1, ui->leInstitutionName->text());
    query.bindValue(2, ui->leDegree->text());
    query.bindValue(3, ui->leSpecialty->text());
    query.bindValue(4, ui->deGraduationDate->date().toString("yyyy-MM-dd"));

    if (!query.exec()) {
        ui->teResult->append("Error: " + query.lastError().text());
        return;
    }

    ui->teResult->append("Record added successfully.");
    loadData();
}

void EducationWindow::on_btnEdit_clicked()
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
    QString sqlstr = "UPDATE Education SET personnel_id = ?, institution_name = ?, degree = ?, specialty = ?, graduation_date = ? WHERE education_id = ?";
    query.prepare(sqlstr);

    // Получаем personnel_id из выбранного элемента в комбобоксе
    int personnelId = ui->cbPersonnelId->currentData().toInt();

    query.bindValue(0, personnelId);
    query.bindValue(1, ui->leInstitutionName->text());
    query.bindValue(2, ui->leDegree->text());
    query.bindValue(3, ui->leSpecialty->text());
    query.bindValue(4, ui->deGraduationDate->date().toString("yyyy-MM-dd"));
    query.bindValue(5, ui->tableWidget->item(currow, 0)->text());

    if (!query.exec()) {
        ui->teResult->append("Error: " + query.lastError().text());
        return;
    }

    ui->teResult->append("Record updated successfully.");
    loadData();
}

void EducationWindow::on_btnDelete_clicked()
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
    QString sqlstr = "DELETE FROM Education WHERE education_id = ?";
    query.prepare(sqlstr);
    query.bindValue(0, ui->tableWidget->item(currow, 0)->text());

    if (!query.exec()) {
        ui->teResult->append("Error: " + query.lastError().text());
        return;
    }

    ui->teResult->append("Record deleted successfully.");
    loadData();
}

void EducationWindow::on_btnBack_clicked()
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
