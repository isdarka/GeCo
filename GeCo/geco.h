#ifndef GECO_H
#define GECO_H

#include <QString>
#include <QtXml>
#include <QtSql>
#include <QSettings>
#include <QStringList>
#include <QDir>
#include "GeCo/Model/gecobean.h"
class GeCo
{
public:
    GeCo();
    void generateXML(QSettings *settings);
    void read(QDomDocument document);
    //QString camelCase(QString str);
    QString path;

private:
    QString camelCase(QString str);
    QStringList prefixes;
    QDomDocument getDefault();
    void write(QDomDocument document);

};

#endif // GECO_H
