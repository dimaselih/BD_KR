#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include "tableselectionwindow.h"
#include "reportselectionwindow.h"

namespace Ui {
class AdminWindow;
}

class AdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminWindow(QWidget *parent = nullptr);
    ~AdminWindow();

private slots:
    void on_btnEditData_clicked();
    void on_btnViewReports_clicked();
    void on_btnLogout_clicked();

private:
    Ui::AdminWindow *ui;
    TableSelectionWindow *tableSelectionWindow;
    ReportSelectionWindow *reportSelectionWindow;
};

#endif // ADMINWINDOW_H
