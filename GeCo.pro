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
    GeCo/geco.cpp

HEADERS  += mainwindow.h \
    DataBase/connection.h \
    mysqlsettings.h \
    Table/Model/fieldbean.h \
    GeCo/Model/gecobean.h \
    GeCo/geco.h

FORMS    += mainwindow.ui \
    mysqlsettings.ui
