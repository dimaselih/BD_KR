#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class DatabaseManager
{
public:
    static DatabaseManager& instance();

    bool connectToDatabase();
    bool isConnected() const;
    QSqlDatabase& database();

private:
    DatabaseManager();
    ~DatabaseManager();

    // Запрещаем копирование и присваивание
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase dbconn;
};

#endif // DATABASEMANAGER_H
