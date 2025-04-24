#ifndef POSITIONWINDOW_H
#define POSITIONWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QTableWidgetItem>

namespace Ui {
class PositionWindow;
}

class PositionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PositionWindow(QWidget *parent = nullptr);
    ~PositionWindow();

private slots:
    void on_btnAdd_clicked();
    void on_btnEdit_clicked();
    void on_btnDelete_clicked();
    void on_btnBack_clicked();
    void on_tableWidget_itemClicked(QTableWidgetItem *item);

private:
    Ui::PositionWindow *ui;

    void loadData();
};

#endif // POSITIONWINDOW_H
