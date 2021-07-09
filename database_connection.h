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
    void initConnection();
    bool openConnetion();
    void closeConnection();

    static QString getDatabasePath();
    static void setDatabasePath(const QString &value);
    static QString getDatabaseType();
    static void setDatabaseType(const QString &value);

    QSqlQuery createQuery();

private:
    DatabaseConnection();
    ~DatabaseConnection();

    QSqlDatabase connection;

    static QString database_path;
    static QString database_type;

    QSqlDatabase createConnection();

protected:
    static DatabaseConnection* instance;
};
}

#endif // DATABASE_CONNECTION_H
