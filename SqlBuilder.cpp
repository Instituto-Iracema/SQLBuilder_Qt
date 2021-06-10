#include "SqlBuilder.h"
#include <QSqlError>
#include <QDebug>

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
 * @brief SqlBuilder::insert
 * @param values
 * @return
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
    this->sql = "";
 * @brief SqlBuilder::destroy
 * @return
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

SqlBuilder* SqlBuilder::executed(int outputMode) {
    this->execute(outputMode);
    return this;
}

/**
 * @brief SqlBuilder::execute
 */
bool SqlBuilder::execute(int outputMode) {
    Database* db = Database::getInstance();
    const bool executed = db->execute(this->sql, this->params);
    const QSqlError queryLastError = db->query->lastError();

    if (
            (outputMode == DebugMode::DebugAll) ||
            (outputMode == DebugMode::DebugErrors && queryLastError.isValid())
            ) {
        qDebug() << "SqlError:" << queryLastError.text();
        qDebug() << "Sql statement:" << this->sql;
        qDebug() << "Params:" << this->params;
    }

    cleanSql();
    return executed;
}

/**
 * @brief SqlBuilder::rows
 * @return
 */
QVariant SqlBuilder::rows(int outputMode, int debugMode) {
    Database* db = Database::getInstance();
    this->execute(debugMode);
    QSqlQuery *query = db->query;

    const int columnsCount = this->columns.size();
    int rowIndex, columnIndex;
    QVariant value;
    QVariantList resultRows, row;
    QVariantMap resultMap;
    QString column;

    // Iterate all rows on result
    for (rowIndex = 0; query->next(); ++rowIndex) {
      for (columnIndex = 0; columnIndex < columnsCount; ++columnIndex) {
          column = this->columns[columnIndex];
          value = query->value(columnIndex);
          if (outputMode == RowOutput::Map) {
              /* Not implemented yet */
          } else if (outputMode == RowOutput::SingleList) {
              resultRows.append(value);
          } else if (outputMode == RowOutput::Grid) {
              row.append(value);
          }
      }
      if (outputMode == RowOutput::Grid) {
          resultRows.append(QVariant(row));
          row = {};
      }
    }

    if (outputMode == RowOutput::Grid || outputMode == RowOutput::SingleList)
        return QVariant(resultRows);
    else if (outputMode == RowOutput::Map)
        return QVariant(resultMap);
    else
        return QVariant();
}

QStringList SqlBuilder::tableColumns() {
    return this->tableInfo()
            ->select({"name"})
            ->rows(RowOutput::SingleList).toStringList();
}

SqlBuilder* SqlBuilder::tableInfo() {
    const QString tableName = QString("PRAGMA_TABLE_INFO('%1')").arg(this->table);
    return new SqlBuilder(tableName);
}

/**
 * @brief SqlBuilder::setSql
 * @param sql
 */
void SqlBuilder::setSql(QString sql) { this->sql = sql; }

/**
 * @brief SqlBuilder::getSql
 * @return
 */
QString SqlBuilder::getSql() { return this->sql; }

/**
 * @brief SqlBuilder::cleanSql
 */
void SqlBuilder::cleanSql() {
   this->sql = "";
}

/**
 * @brief db
 * @param table
 * @return
 */
SqlBuilder* queryBuilder(QString table) {
  return new SqlBuilder(table);
}
