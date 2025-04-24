#include "adminwindow.h"
#include "ui_adminwindow.h"
#include <QSignalBlocker>

AdminWindow::AdminWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminWindow),
    tableSelectionWindow(nullptr),
    reportSelectionWindow(nullptr)
{
    ui->setupUi(this);

    // Подключаем сигналы к слотам
    connect(ui->btnEditData, &QPushButton::clicked, this, &AdminWindow::on_btnEditData_clicked);
    connect(ui->btnViewReports, &QPushButton::clicked, this, &AdminWindow::on_btnViewReports_clicked);
    connect(ui->btnLogout, &QPushButton::clicked, this, &AdminWindow::on_btnLogout_clicked);
}

AdminWindow::~AdminWindow()
{
    delete ui;
    if (tableSelectionWindow)
        delete tableSelectionWindow;
    if (reportSelectionWindow)
        delete reportSelectionWindow;
}

void AdminWindow::on_btnEditData_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnEditData);

    if (!tableSelectionWindow) {
        tableSelectionWindow = new TableSelectionWindow(this);
    }
    tableSelectionWindow->show();
    this->hide();
}

void AdminWindow::on_btnViewReports_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnViewReports);

    if (!reportSelectionWindow) {
        reportSelectionWindow = new ReportSelectionWindow(this);
    }
    reportSelectionWindow->show();
    this->hide();
}

void AdminWindow::on_btnLogout_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnLogout);

    // Показываем родительское окно, если оно существует
    if (parent()) {
        QWidget* parentWidget = qobject_cast<QWidget*>(parent());
        if (parentWidget) {
            parentWidget->show();
        }
    }

    // Закрываем текущее окно
    this->close();
}
