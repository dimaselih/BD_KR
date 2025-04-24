#include "userwindow.h"
#include "ui_userwindow.h"
#include <QSignalBlocker>

UserWindow::UserWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserWindow),
    reportSelectionWindow(nullptr)
{
    ui->setupUi(this);

    // Подключаем сигналы к слотам
    connect(ui->btnViewReports, &QPushButton::clicked, this, &UserWindow::on_btnViewReports_clicked);
    connect(ui->btnLogout, &QPushButton::clicked, this, &UserWindow::on_btnLogout_clicked);
}

UserWindow::~UserWindow()
{
    delete ui;
    if (reportSelectionWindow)
        delete reportSelectionWindow;
}

void UserWindow::on_btnViewReports_clicked()
{
    // Блокируем сигналы
    QSignalBlocker blocker(ui->btnViewReports);

    if (!reportSelectionWindow) {
        reportSelectionWindow = new ReportSelectionWindow(this);
    }
    reportSelectionWindow->show();
    this->hide();
}

void UserWindow::on_btnLogout_clicked()
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
