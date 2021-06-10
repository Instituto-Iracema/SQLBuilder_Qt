#include <QCoreApplication>
#include<QDebug>

#include <SqlBuilder.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    // return a.exec();

    /**
     * Select example.
     */
    // queryBuilder("users")
    //   ->select({"id", "name", "registration"})
    //   ->where({"id", "=", {1})
    //   ->rows();

    /**
     * Insert example.
     */
    // QVariantMap values =  {
    //     {"name", "Chaves"},
    //     {"registration", "123456"},
    // };

    // queryBuilder("users")
    //         ->insert(values)
    //         ->execute();

    /**
     * Delete example.
     */
    // queryBuilder("users")
    // ->destroy()
    // ->where("id", "=", {14})
    // ->execute();

    /**
     * Update example.
     */

   // QVariantMap values;

   // values["name"] = "Ñoño";
   // values["registration"] = "654321";

   // queryBuilder("users")
   // ->update(values)->where({"id", "=", "4"})->execute();

    return 0;
}
