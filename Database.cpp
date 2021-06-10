#include "Database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

Database* Database::instance = nullptr;

Database::Database()
{
  this->connection = this->createConnection();
  this->query = this->createQuery();
}

Database::~Database()
{
  this->closeConnection();
}

bool Database::execute(QString sql, QVariantMap params) {
  QSqlQuery *query = this->query;
  query->prepare(sql);
  for (QString param : params.keys()) {
   query->bindValue(":" + param, params[param]);
  }
  return query->exec();
}

void Database::closeConnection()
{
    this->query->clear();
    this->connection.close();
}

Database* Database::getInstance() {
    if (Database::instance == 0)
    {
        Database::instance = new Database();
    }
    return Database::instance;
}

/*
 * @brief Database::createQuery
 * @return
 */
QSqlQuery* Database::createQuery() {
	return new QSqlQuery(this->connection);
}

/**
 * Returns an instance of database connection.
 * @brief Database::createConnection
 * @return
 */
QSqlDatabase Database::createConnection() {
    /**
     * Instanciate database connection object
     * using SQLITE driver.
     * @brief db
     */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/tmp/oxygiga.db");

    if (!db.open()) {
      /**
       * Throws an exception if had some error on connection attempt.
       * @throws exception
       */
       throw std::runtime_error("Error connecting to database.");
    }

    return db;
}
