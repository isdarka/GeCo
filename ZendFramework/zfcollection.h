#ifndef ZFCOLLECTION_H
#define ZFCOLLECTION_H

#include "GeCo/Model/gecobean.h"
#include <QVector>
#include <CodeGenerator/docblock.h>
#include <CodeGenerator/method.h>
#include <CodeGenerator/phpclass.h>
#include <CodeGenerator/propertie.h>
#include "DataBase/Model/columnbean.h"
#include "DataBase/Model/tablecatalog.h"


#include <QDebug>
#include <QDateTime>
#include <QFile>

class ZfCollection
{
public:
    ZfCollection(GeCoBean model, QVector<GeCoBean> models);
    void generate();
    void write(QString str);
private:
    GeCoBean getByExntendName(QString extend);
    GeCoBean model;
    QVector<GeCoBean> models;
    QVector<ColumnBean> columns;
    QString ucfirst(QString str);
    PhpClass code;
    Docblock classDocBlock();
    Method getConstruct();
    ColumnBean getPrimaryKey();
    Method getAppend();
    Method getIsEmpty();
    Method getRemove();
};

#endif // ZFCOLLECTION_H
