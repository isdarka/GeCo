#ifndef ZFVIEW_H
#define ZFVIEW_H


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
#include <QDir>
class ZfView
{
public:

    ZfView(GeCoBean model, QVector<GeCoBean> models);

    QString generateIndex();
    void writeIndex(QString str);
    QString generateForm();
    void writeForm(QString str);

    QString generateHistory();
    void writeHistory(QString str);
private:
    GeCoBean getByExntendName(QString extend);
    GeCoBean model;
    QVector<GeCoBean> models;
    QVector<ColumnBean> columns;
    QString ucfirst(QString str);
    QString lcFirst(QString str);
    PhpClass code;
};

#endif // ZFVIEW_H
