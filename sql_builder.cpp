#include "sql_builder.h"
#include <QDebug>

using SqlBuilder = SQLBuilder::SqlBuilder;

SqlBuilder::SqlBuilder(QString table)
{
  this->table = table;
}

/**
 * @brief SqlBuilder::select
 * @param columns
 * @return
 */
SqlBuilder* SqlBuilder::select(QStringList columns) {
    this->sql += "SELECT " + columns.join(",") + " FROM " + this->table;

    foreach (QString column, columns) {
        if (column != "*") {
            this->columns.append(column);
        } else {
            this->columns.append(this->tableColumns());
        }
    }

    return this;
}

SqlBuilder* SqlBuilder::where(QString column, QString sqlOperator, QVariantList values) {
    this->sql += QString(" WHERE %1 %2 ").arg(column, sqlOperator);

    if (sqlOperator.toUpper() == "BETWEEN")
        this->sql += ":where_0 AND :where_1";
    else if (sqlOperator.toUpper() == "IN")
        this->sql += "(";

    for (int index = 0; index < values.length(); ++index) {
        // sets params["where_i"] to values[i]
            this->params[QString("where_%1").arg(index)] = values[index];
        // constructs :where_0,:where_1,...,where_n
        if (sqlOperator.toUpper() != "BETWEEN") {
            this->sql += QString(":where_%1").arg(index);
            if (index != values.length() - 1) {
                this->sql += ',';
            }
        }
    }

    if (sqlOperator.toUpper() == "IN")
        this->sql += ")";

    return this;
}

/**
 * @brief insert on current sql the core of a delete sql query
 * @param mapColumnToValue
 * @return This
 */
SqlBuilder* SqlBuilder::insert(QVariantMap mapColumnToValue) {
    this->sql += "INSERT INTO " + this->table;
    this->params = mapColumnToValue;

    QString column, columnsStatement, placeholdersStatement;
    const QStringList columnsList = mapColumnToValue.keys();
    const QString lastColumn = columnsList.last();

    foreach (column, columnsList) {
        columnsStatement += column;
        placeholdersStatement += ':' + column;
        if (column != lastColumn) {
            columnsStatement += ',';
            placeholdersStatement += ',';
        }
    }

    this->sql += QString(" (%1) VALUES (%2)")
            .arg(columnsStatement, placeholdersStatement);

    return this;
}

/**
 * @brief insert on current sql the core of a delete sql query
 * @return This
 */
SqlBuilder* SqlBuilder::destroy() {
    this->sql += "DELETE FROM " + this->table;
    return this;
}

/**
 * @brief SqlBuilder::update
 * @param values
 * @return
 */
SqlBuilder* SqlBuilder::update(const QVariantMap mapColumnToValue) {
    this->sql += "UPDATE " + this->table + " SET ";
    this->params = mapColumnToValue;

    QString column;
    const QStringList columns = mapColumnToValue.keys();

    foreach (column, columns) {
        this->sql += column + "=" + ":" + column;
        if (column != columns.last())
            this->sql += ',';
    }

    return this;
}

/**
 * @brief call the execute function
 * @param outputMode The format to output
 * @return This
 */
bool SqlBuilder::executed(int outputMode) {
    DatabaseConnection::getInstance()->openConnetion();
    QSqlQuery query = DatabaseConnection::getInstance()->createQuery();
    query.prepare(sql);
    foreach(auto param, params.keys()) {
        query.bindValue(":" + param, params[param]);
    }

    bool result = query.exec();

    const QSqlError queryLastError = query.lastError();

    if ((outputMode == DebugMode::DebugAll) ||(outputMode == DebugMode::DebugErrors && queryLastError.isValid())) {
        qDebug() << "SqlError:" << queryLastError.text();
        qDebug() << "Sql statement:" << this->sql;
        qDebug() << "Params:" << this->params;
    }

    cleanSql();
    return result;
}

/**
 * @brief Execute the current sql query
 * @param outputMode The format to output
 * @param lastInsertId A pointer to last inserted row id on database
 * @return True if the query was executed and false if it fail
 */
QSqlQuery SqlBuilder::execute(int outputMode,int* lastInsertId) {
    DatabaseConnection::getInstance()->openConnetion();
    QSqlQuery query = DatabaseConnection::getInstance()->createQuery();
    query.prepare(sql);
    foreach(auto param, params.keys()) {
        query.bindValue(":" + param, params[param]);
    }

    query.exec();

    if(lastInsertId!=nullptr)
        *lastInsertId = query.lastInsertId().toInt();

    const QSqlError queryLastError = query.lastError();

    if ((outputMode == DebugMode::DebugAll) ||(outputMode == DebugMode::DebugErrors && queryLastError.isValid())) {
        qDebug() << "SqlError:" << queryLastError.text();
        qDebug() << "Sql statement:" << this->sql;
        qDebug() << "Params:" << this->params;
    }

    cleanSql();
    return query;
}

/**
 * @brief Execute the current sql query and format the output
 * @param outputMode The format to output
 * @param debugMode Define what will be printed on application output
 * @return formated response of the query
 */
QVariant SqlBuilder::rows(int outputMode, int debugMode) {
    QSqlQuery query = this->execute(debugMode);

    const int columnsCount = this->columns.size();
    int rowIndex, columnIndex;
    QVariant value;
    QVariantList resultRows, row,resultMap;
    QVariantMap rowMap;
    QString column;

    // Iterate all rows on result
    for (rowIndex = 0; query.next(); ++rowIndex) {
      for (columnIndex = 0; columnIndex < columnsCount; ++columnIndex) {
          column = this->columns[columnIndex];
          value = query.value(columnIndex);
          if (outputMode == RowOutput::Map) {
              rowMap.insert(column,value);
          } else if (outputMode == RowOutput::SingleList) {
              resultRows.append(value);
          } else if (outputMode == RowOutput::Grid) {
              row.append(value);
          }
      }
      if (outputMode == RowOutput::Grid) {
          resultRows.append(QVariant(row));
          row = {};
      } else if(outputMode == RowOutput::Map) {
          resultMap.append(rowMap);
          rowMap = {};
      }
    }

    DatabaseConnection::getInstance()->closeConnection();

    if (outputMode == RowOutput::Grid || outputMode == RowOutput::SingleList)
        return QVariant(resultRows);
    else if (outputMode == RowOutput::Map)
        return QVariant(resultMap);
    else
        return QVariant();
}

/**
 * @brief SqlBuilder::tableColumns
 * @return
 */
QStringList SqlBuilder::tableColumns() {
    return this->tableInfo()
            ->select({"name"})
            ->rows(RowOutput::SingleList).toStringList();
}

/**
 * @brief SqlBuilder::tableInfo
 * @return
 */
SqlBuilder* SqlBuilder::tableInfo() {
    const QString tableName = QString("PRAGMA_TABLE_INFO('%1')").arg(this->table);
    return new SqlBuilder(tableName);
}

/**
 * @brief set a new value to sql query
 * @param The new sql query
 */
void SqlBuilder::setSql(QString sql) { this->sql = sql; }

/**
 * @brief get the current sql quary
 * @return The sql query
 */
QString SqlBuilder::getSql() { return this->sql; }

/**
 * @brief define the sql query to empty string
 */
void SqlBuilder::cleanSql() {
   this->sql = "";
}

/**
 * @brief return table name
 * @return The table name
 */
QString SQLBuilder::SqlBuilder::getTableName(){ return this->table; }
