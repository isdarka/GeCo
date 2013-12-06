#-------------------------------------------------
#
# Project created by QtCreator 2013-12-03T11:12:34
#
#-------------------------------------------------

QT       += core gui sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GeCo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    DataBase/connection.cpp \
    mysqlsettings.cpp \
    Table/Model/fieldbean.cpp \
    GeCo/Model/gecobean.cpp \
    GeCo/geco.cpp \
    prefixes.cpp \
    CodeGenerator/propertie.cpp \
    CodeGenerator/phpclass.cpp \
    CodeGenerator/method.cpp \
    CodeGenerator/docblock.cpp \
    ZendFramework/zfcollection.cpp \
    ZendFramework/zfbean.cpp \
    DataBase/Model/tablecatalog.cpp \
    DataBase/Model/columnbean.cpp \
    ZendFramework/zfexception.cpp \
    ZendFramework/zffactory.cpp \
    ZendFramework/zfmetadata.cpp \
    ZendFramework/zfcatalog.cpp \
    ZendFramework/zfcrud.cpp \
    ZendFramework/zfquery.cpp

HEADERS  += mainwindow.h \
    DataBase/connection.h \
    mysqlsettings.h \
    Table/Model/fieldbean.h \
    GeCo/Model/gecobean.h \
    GeCo/geco.h \
    prefixes.h \
    CodeGenerator/propertie.h \
    CodeGenerator/phpclass.h \
    CodeGenerator/method.h \
    CodeGenerator/docblock.h \
    ZendFramework/zfcollection.h \
    ZendFramework/zfbean.h \
    DataBase/Model/tablecatalog.h \
    DataBase/Model/columnbean.h \
    ZendFramework/zfexception.h \
    ZendFramework/zffactory.h \
    ZendFramework/zfmetadata.h \
    ZendFramework/zfcatalog.h \
    ZendFramework/zfcrud.h \
    ZendFramework/zfquery.h

FORMS    += mainwindow.ui \
    mysqlsettings.ui \
    prefixes.ui
