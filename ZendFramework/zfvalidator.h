#ifndef ZFVALIDATOR_H
#define ZFVALIDATOR_H
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


class ZfValidator
{
public:
//    ZfValidator();
    ZfValidator(GeCoBean model, QVector<GeCoBean> models);

    void generate();
    void write(QString str);
private:
    GeCoBean getByExntendName(QString extend);
    GeCoBean model;
    QVector<GeCoBean> models;
    QVector<ColumnBean> columns;
    QString ucfirst(QString str);
    QString lcFirst(QString str);
    PhpClass code;
};

#endif // ZFVALIDATOR_H
