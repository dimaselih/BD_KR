#ifndef ASSIGNMENTREPORTWINDOW_H
#define ASSIGNMENTREPORTWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QComboBox>
#include <QDate>

namespace Ui {
class AssignmentReportWindow;
}

class AssignmentReportWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AssignmentReportWindow(QWidget *parent = nullptr);
    ~AssignmentReportWindow();

private slots:
    void on_btnBack_clicked();
    void on_btnSearch_clicked();
    void on_cbFilterType_currentIndexChanged(int index);

private:
    Ui::AssignmentReportWindow *ui;

    void loadData();
    void loadFilterComboBoxes();
    void updateFilterVisibility(int filterType);
};

#endif // ASSIGNMENTREPORTWINDOW_H
