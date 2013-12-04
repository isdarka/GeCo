#include "gecobean.h"

GeCoBean::GeCoBean()
{
}

void GeCoBean::setName(QString str)
{
    this->name = str;
}

void GeCoBean::setTable(QString str)
{
    this->table = str;
}

void GeCoBean::setModule(QString str)
{
    this->module = str;
}

void GeCoBean::setExtend(QString str)
{
    this->extend = str;
}

void GeCoBean::addRelation(QString str)
{
    this->relations.append(str);
}

void GeCoBean::setForm(bool boolean)
{
    this->form = boolean;
}

void GeCoBean::setModel(bool boolean)
{
    this->model = boolean;
}

void GeCoBean::setController(bool boolean)
{
    this->controller = boolean;
}

void GeCoBean::setView(bool boolean)
{
    this->view = boolean;
}

void GeCoBean::setLog(bool boolean)
{
    this->log = boolean;
}

void GeCoBean::setLogName(QString str)
{
    this->logName = str;
}


QString GeCoBean::getName()
{
    return this->name;
}

QString GeCoBean::getTable()
{
    return this->table;
}

QString GeCoBean::getModule()
{
    return this->module;
}

QString GeCoBean::getExtend()
{
    return this->extend;
}

QStringList GeCoBean::getRelations()
{
    return this->relations;
}

bool GeCoBean::getModel()
{
    return this->model;
}

bool GeCoBean::getController()
{
    return this->controller;
}

bool GeCoBean::getView()
{
    return this->view;
}

bool GeCoBean::getLog()
{
    return this->log;
}

QString GeCoBean::getLogName()
{
    return this->logName;
}

bool GeCoBean::getForm()
{
    return this->form;
}
