#include "personnelreportwindow.h"
#include "ui_personnelreportwindow.h"
#include <QSignalBlocker>
#include <QWidget>
#include "databasemanager.h"

PersonnelReportWindow::PersonnelReportWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PersonnelReportWindow)
{
    ui->setupUi(this);

    // Настройка таблицы
    ui->tableWidget->setColumnCount(5); // Устанавливаем количество столбцов
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID" << "ФИО" << "Пол" << "Дата рождения" << "Специальность");
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Отключаем все сигналы
    disconnect(ui->btnBack, nullptr, nullptr, nullptr);
    disconnect(ui->btnSearch, nullptr, nullptr, nullptr);

    // Подключаем сигналы только один раз
    connect(ui->btnBack, &QPushButton::clicked, this, &PersonnelReportWindow::on_btnBack_clicked);
    connect(ui->btnSearch, &QPushButton::clicked, this, &PersonnelReportWindow::on_btnSearch_clicked);

    // Настройка комбобокса для пола
    setupGenderComboBox();

    // Подключаемся к базе данных и загружаем данные при создании формы
    loadData(); // Загружаем все данные при открытии формы
}

PersonnelReportWindow::~PersonnelReportWindow()
{
    delete ui;
}

void PersonnelReportWindow::setupGenderComboBox()
{
    ui->cbGender->clear();
    ui->cbGender->addItem("Все", "");
    ui->cbGender->addItem("М", "М");
    ui->cbGender->addItem("Ж", "Ж");
}

void PersonnelReportWindow::loadData(const QString &searchText)
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    if (!DatabaseManager::instance().isConnected() && !DatabaseManager::instance().connectToDatabase()) {
        return;
    }

    QSqlQuery query(DatabaseManager::instance().database());
    QString sqlstr;
    QString genderFilter = ui->cbGender->currentData().toString();

    if (searchText.isEmpty() && genderFilter.isEmpty()) {
        // Загрузить все записи
        sqlstr = "SELECT personnel_id, full_name, gender, birth_date, specialty "
                 "FROM Personnel "
                 "ORDER BY full_name";
        if (!query.exec(sqlstr)) {
            return;
        }
    } else {
        // Применяем фильтры
        QStringList conditions;
        if (!searchText.isEmpty()) {
            conditions << "(full_name ILIKE ? OR specialty ILIKE ?)";
        }
        if (!genderFilter.isEmpty()) {
            conditions << "gender = ?";
        }

        sqlstr = "SELECT personnel_id, full_name, gender, birth_date, specialty "
                 "FROM Personnel ";

        if (!conditions.isEmpty()) {
            sqlstr += "WHERE " + conditions.join(" AND ") + " ";
        }

        sqlstr += "ORDER BY full_name";

        query.prepare(sqlstr);

        int paramIndex = 0;
        if (!searchText.isEmpty()) {
            QString likePattern = "%" + searchText + "%";
            query.bindValue(paramIndex++, likePattern);
            query.bindValue(paramIndex++, likePattern);
        }
        if (!genderFilter.isEmpty()) {
            query.bindValue(paramIndex++, genderFilter);
        }

        if (!query.exec()) {
            return;
        }
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

    // Автоматическое выравнивание столбцов
    ui->tableWidget->resizeColumnsToContents();

    // Обновляем таблицу
    ui->tableWidget->update();

    // Обновляем статус
    ui->statusbar->showMessage(QString("Найдено записей: %1").arg(row));
}

void PersonnelReportWindow::on_btnSearch_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnSearch);

    QString searchText = ui->leSearch->text();
    loadData(searchText);
}

void PersonnelReportWindow::on_btnBack_clicked()
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
