TEMPLATE = app

QT -= gui
QT += sql
TEMPLATE = lib
DEFINES += QTSQLBUILDER_LIBRARY

CONFIG += c++11
CONFIG += staticlib
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
SOURCES += \
    database_connection.cpp \
    sql_builder.cpp

HEADERS += \
    QtSqlBuilder_global.h \
    database_connection.h \
    sql_builder.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
