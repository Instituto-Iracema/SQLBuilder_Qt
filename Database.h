#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariantMap>

class Database
{
public:
    static Database* getInstance();
    QSqlDatabase connection;
    QSqlQuery *query;
    void closeConnection();
    bool execute(QString sql, QVariantMap params = {});

private:
    Database();
    ~Database();

    QSqlQuery* createQuery();
    QSqlDatabase createConnection();
protected:
    static Database* instance;
};

#endif // DATABASE_H
