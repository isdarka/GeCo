#-------------------------------------------------
#
# Project created by QtCreator 2013-12-03T11:12:34
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GeCo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    DataBase/connection.cpp \
    mysqlsettings.cpp

HEADERS  += mainwindow.h \
    DataBase/connection.h \
    mysqlsettings.h

FORMS    += mainwindow.ui \
    mysqlsettings.ui
