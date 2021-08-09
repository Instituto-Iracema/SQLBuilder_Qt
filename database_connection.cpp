#include "database_connection.h"
#include <QDebug>

using DatabaseConnection = SQLBuilder::DatabaseConnection;

DatabaseConnection* DatabaseConnection::instance = nullptr;

QString DatabaseConnection::database_type = "QSQLITE";
QString DatabaseConnection::database_path = "/tmp/slq_builder.db";

DatabaseConnection::DatabaseConnection(){}

DatabaseConnection::~DatabaseConnection()
{
  this->closeConnection();
}

/**
 * @brief Clear the querry attribute value and close database connection
 */
void DatabaseConnection::closeConnection()
{
    this->connection.close();
}

/**
 * @brief DatabaseConnection::getInstance
 * @return
 */
DatabaseConnection* DatabaseConnection::getInstance()
{
    if (DatabaseConnection::instance == 0)
    {
        DatabaseConnection::instance = new DatabaseConnection();
    }
    return DatabaseConnection::instance;
}

/**
 * @brief DatabaseConnection::openConnection
 */
void DatabaseConnection::initConnection()
{
    this->connection = this->createConnection();
}

/**
 * @brief Create a pointer to a QSqlQuery created from current connection
 * @return The QSqlQuery pointer
 */
QSqlQuery DatabaseConnection::createQuery() {
    return QSqlQuery(this->connection);
}

/**
 * @brief Configure the connection attribute
 * @return the db instance
 */
QSqlDatabase DatabaseConnection::createConnection() {
    /**
     * @brief Instanciate database connection object
     * using SQLITE driver.
     */
    connection = QSqlDatabase::addDatabase(database_type);
    connection.setDatabaseName(database_path);

    openConnetion();
    closeConnection();

    return connection;
}

/**
 * @brief Try open the database file
 * @return True if database was opened and throw exception if a error occur
 */
bool DatabaseConnection::openConnetion()
{
    if (!this->connection.open()) {
      /**
       * Throws an exception if had some error on connection attempt.
       * @throws exception
       */
//       throw std::runtime_error(QString("Error trying connect to database %1.").arg(database_path).toUtf8());
    }
    return  true;
}

/**
 * @brief Get database type attribute value
 * @return The database type attribute value
 */
QString DatabaseConnection::getDatabaseType()
{
    return database_type;
}

/**
 * @brief DatabaseConnection::setDatabaseType
 * @param value
 */
void DatabaseConnection::setDatabaseType(const QString &value)
{
    database_type = value;
}

/**
 * @brief Get database path attribute value
 * @return The database path attribute value
 */
QString DatabaseConnection::getDatabasePath()
{
    return database_path;
}

/**
 * @brief DatabaseConnection::setDatabasePath
 * @param value
 */
void DatabaseConnection::setDatabasePath(const QString &value)
{
    database_path = value;
}
