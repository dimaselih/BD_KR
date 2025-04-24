#ifndef POSITIONREPORTWINDOW_H
#define POSITIONREPORTWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QComboBox>
#include <QDate>

namespace Ui {
class PositionReportWindow;
}

class PositionReportWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PositionReportWindow(QWidget *parent = nullptr);
    ~PositionReportWindow();

private slots:
    void on_btnBack_clicked();
    void on_btnSearch_clicked();

private:
    Ui::PositionReportWindow *ui;

    void loadData(int positionId = -1);
    void loadPositionComboBox();
};

#endif // POSITIONREPORTWINDOW_H
