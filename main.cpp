#include <QCoreApplication>
#include<QDebug>

#include <sql_builder.h>

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    /*
    * Select example.
    */
    SQLBuilder::SqlBuilder("users")
        .select({"id", "name", "registration"})
        ->where("id", "=", {1})
        ->rows();

    /*
    * Insert example.
    */
    QVariantMap values =  {
        {"name", "Chaves"},
        {"registration", "123456"},
    };

    SQLBuilder::SqlBuilder("users")
        .insert(values)
        ->execute();

    /*
    * Delete example.
    */
    SQLBuilder::SqlBuilder("users")
        .destroy()
        ->where("id", "=", {14})
        ->execute();

    /*
    * Update example.
    */
    QVariantMap values2;

    values["name"] = "Ñoño";
    values["registration"] = "654321";

    SQLBuilder::SqlBuilder("users")
        .update(values2)
        ->where("id", "=", {"4"})->execute();

    return 0;
}
