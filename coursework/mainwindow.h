#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include "adminwindow.h"
#include "userwindow.h"
#include "databasemanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnLogin_clicked();

private:
    Ui::MainWindow *ui;
    AdminWindow *adminWindow;
    UserWindow *userWindow;

    bool authenticateUser(const QString &username, const QString &password, QString &role);
};

#endif // MAINWINDOW_H
