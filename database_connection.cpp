#include "database_connection.h"
#include <QDebug>

using DatabaseConnection = SQLBuilder::DatabaseConnection;

DatabaseConnection* DatabaseConnection::instance = nullptr;

DatabaseConnection::DatabaseConnection()
{
  this->connection = this->createConnection();
  this->query = this->createQuery();
}

DatabaseConnection::~DatabaseConnection()
{
  this->closeConnection();
}

bool DatabaseConnection::execute(QString sql, QVariantMap params) {
  QSqlQuery *query = this->query;
  query->prepare(sql);
  foreach(auto param, params.keys()) {
   query->bindValue(":" + param, params[param]);
  }
  return query->exec();
}

void DatabaseConnection::closeConnection()
{
    this->query->clear();
    this->connection.close();
}

DatabaseConnection* DatabaseConnection::getInstance() {
    if (DatabaseConnection::instance == 0)
    {
        DatabaseConnection::instance = new DatabaseConnection();
    }
    return DatabaseConnection::instance;
}

/*
 * @brief Database::createQuery
 * @return
 */
QSqlQuery* DatabaseConnection::createQuery() {
	return new QSqlQuery(this->connection);
}

/**
 * Returns an instance of database connection.
 * @brief Database::createConnection
 * @return
 */
QSqlDatabase DatabaseConnection::createConnection() {
    /**
     * Instanciate database connection object
     * using SQLITE driver.
     * @brief db
     */
    QSqlDatabase db = QSqlDatabase::addDatabase(DATABASE_TYPE);
    db.setDatabaseName(DATABASE_PATH);

    if (!db.open()) {
      /**
       * Throws an exception if had some error on connection attempt.
       * @throws exception
       */
       throw std::runtime_error(QString("Error trying connect to database %1.").arg(DATABASE_PATH).toUtf8());
    }

    return db;
}
