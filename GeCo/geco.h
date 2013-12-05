#ifndef GECO_H
#define GECO_H

#include <QString>
#include <QtXml>
#include <QtSql>
#include <QSettings>
#include <QStringList>
#include <QDir>
#include "GeCo/Model/gecobean.h"
#include "ZendFramework/zfbean.h"
#include "ZendFramework/zfcollection.h"
#include "ZendFramework/zfexception.h"
#include "ZendFramework/zffactory.h"

class GeCo
{
public:
    GeCo();
    void generateXML(QSettings *settings);
    void read(QDomDocument document);
    //QString camelCase(QString str);
    QString path;
    void generateCode(QSettings *settings);
    void generateModel(GeCoBean model);
private:
    QString camelCase(QString str);
    QStringList prefixes;
    QDomDocument getDefault();    
    void createStructure();
    void write(QDomDocument document);
    QSettings *settings;
     QVector<GeCoBean> beans;
};

#endif // GECO_H
