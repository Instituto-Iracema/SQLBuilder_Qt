#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariantMap>

#define DATABASE_TYPE "QSQLITE"
#define DATABASE_PATH "/tmp/slq_builder.db"

namespace SQLBuilder{
class DatabaseConnection
{
public:
    static DatabaseConnection* getInstance();
    QSqlDatabase connection;
    QSqlQuery *query;
    void closeConnection();
    bool execute(QString sql, QVariantMap params = {});

private:
    DatabaseConnection();
    ~DatabaseConnection();

    QSqlQuery* createQuery();
    QSqlDatabase createConnection();
protected:
    static DatabaseConnection* instance;
};
}

#endif // DATABASE_CONNECTION_H
