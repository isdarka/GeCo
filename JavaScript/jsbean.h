#ifndef JSBEAN_H
#define JSBEAN_H
#include <GeCo/Model/gecobean.h>
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
#include <CodeGenerator/javascriptclass.h>
class JsBean
{
public:    
    JsBean(GeCoBean model, QVector<GeCoBean> models);
    void generate();
    void write(QString str);
private:
    GeCoBean getByExntendName(QString extend);
    GeCoBean model;
    QVector<GeCoBean> models;
    QVector<ColumnBean> columns;
    QString ucfirst(QString str);
    QString lcFirst(QString str);
    JavaScriptClass code;
};

#endif // JSBEAN_H
