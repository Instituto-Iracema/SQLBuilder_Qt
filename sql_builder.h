#ifndef SQL_BUILDER_H
#define SQL_BUILDER_H

#include <QString>
#include <QStringList>
#include <QVariantMap>
#include <QSqlError>

#include "database_connection.h"

namespace SQLBuilder{
class SqlBuilder
{
protected:
    QString sql;
    QString table;
    QVariantMap params;
    QStringList columns;
public:
    SqlBuilder(QString table);
    SqlBuilder* select(QStringList columns = {"*"});
    SqlBuilder* where(QString column, QString sqlOperator, QVariantList values);
    SqlBuilder* where(QString query);
    SqlBuilder* insert(const QVariantMap mapColumnToValue);
    SqlBuilder* destroy();
    SqlBuilder* update(const QVariantMap mapColumnToValue);
    SqlBuilder* tableInfo();
    bool executed(int outputMode=DebugMode::DebugErrors);

    QSqlQuery execute(int debugMode=DebugMode::DebugErrors,int* lastInsertId = nullptr);
    QVariant rows(int outputMode=RowOutput::Map, int debugMode=DebugMode::DebugErrors);

    QStringList tableColumns();
    QString getTableName();

    void setSql(QString sql);
    QString getSql();
    void cleanSql();

    enum DebugMode {
        DebugNothing,
        DebugErrors,
        DebugAll,
    };

    enum RowOutput {
        Grid,
        SingleList,
        Map,
    };
};
}


#endif // SQL_BUILDER_H
