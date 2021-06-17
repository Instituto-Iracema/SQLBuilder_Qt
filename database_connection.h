#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariantMap>

namespace SQLBuilder{
class DatabaseConnection
{
public:
    static DatabaseConnection* getInstance();
    QSqlDatabase connection;
    QSqlQuery *query;
    void openConnection();
    void closeConnection();
    bool execute(QString sql, QVariantMap params = {},int* lastInsertId = nullptr);

    static QString getDatabasePath();
    static void setDatabasePath(const QString &value);
    static QString getDatabaseType();
    static void setDatabaseType(const QString &value);

private:
    DatabaseConnection();
    ~DatabaseConnection();

    static QString database_path;
    static QString database_type;

    QSqlQuery* createQuery();
    QSqlDatabase createConnection();

    bool openDatabase();

protected:
    static DatabaseConnection* instance;
};
}

#endif // DATABASE_CONNECTION_H
