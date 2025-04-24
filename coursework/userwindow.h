#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QMainWindow>
#include "reportselectionwindow.h"

namespace Ui {
class UserWindow;
}

class UserWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserWindow(QWidget *parent = nullptr);
    ~UserWindow();

private slots:
    void on_btnViewReports_clicked();
    void on_btnLogout_clicked();

private:
    Ui::UserWindow *ui;
    ReportSelectionWindow *reportSelectionWindow;
};

#endif // USERWINDOW_H
