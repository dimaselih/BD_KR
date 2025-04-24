#include "databasemanager.h"

DatabaseManager::DatabaseManager()
{
    connectToDatabase();
}

DatabaseManager::~DatabaseManager()
{
    if (dbconn.isOpen())
        dbconn.close();

    QString connectionName = dbconn.connectionName();
    dbconn = QSqlDatabase(); // Освобождаем соединение
    QSqlDatabase::removeDatabase(connectionName); // Удаляем соединение
}

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::connectToDatabase()
{
    if (!dbconn.isOpen())
    {
        dbconn = QSqlDatabase::addDatabase("QPSQL", "AppConnection");
        dbconn.setDatabaseName("AIS");
        dbconn.setHostName("localhost");
        dbconn.setUserName("student");
        dbconn.setPassword("1");
        dbconn.setConnectOptions("requiressl=0");

        if (!dbconn.open())
        {
            qDebug() << "Error connecting to database:" << dbconn.lastError().text();
            return false;
        }

        qDebug() << "Connected to database";
        return true;
    }

    return true;
}

bool DatabaseManager::isConnected() const
{
    return dbconn.isOpen();
}

QSqlDatabase& DatabaseManager::database()
{
    return dbconn;
}
