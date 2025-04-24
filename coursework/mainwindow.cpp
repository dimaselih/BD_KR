#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , adminWindow(nullptr)
    , userWindow(nullptr)
{
    ui->setupUi(this);

    // Отключаем все существующие соединения для кнопки btnLogin
    disconnect(ui->btnLogin, nullptr, nullptr, nullptr);

    // Подключаем сигнал только один раз
    connect(ui->btnLogin, &QPushButton::clicked, this, &MainWindow::on_btnLogin_clicked);

    // Подключаемся к базе данных при запуске приложения
    DatabaseManager::instance().connectToDatabase();
}

MainWindow::~MainWindow()
{
    delete ui;
    if (adminWindow)
        delete adminWindow;
    if (userWindow)
        delete userWindow;
}

bool MainWindow::authenticateUser(const QString &username, const QString &password, QString &role)
{
    if (!DatabaseManager::instance().isConnected() && !DatabaseManager::instance().connectToDatabase())
        return false;

    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT role FROM Users WHERE username = ? AND password = ?");
    query.bindValue(0, username);
    query.bindValue(1, password);

    if (!query.exec())
    {
        ui->statusbar->showMessage("Authentication error: " + query.lastError().text());
        return false;
    }

    if (query.next())
    {
        role = query.value("role").toString();
        return true;
    }

    return false;
}

void MainWindow::on_btnLogin_clicked()
{
    QString username = ui->leUsername->text();
    QString password = ui->lePassword->text();

    if (username.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "Login Error", "Please enter username and password");
        return;
    }

    QString role;
    if (authenticateUser(username, password, role))
    {
        if (role == "admin")
        {
            // Открываем окно администратора
            if (!adminWindow)
                adminWindow = new AdminWindow(this);
            adminWindow->show();
            this->hide();
        }
        else if (role == "user")
        {
            // Открываем окно пользователя
            if (!userWindow)
                userWindow = new UserWindow(this);
            userWindow->show();
            this->hide();
        }
    }
    else
    {
        QMessageBox::warning(this, "Login Error", "Invalid username or password");
    }
}
