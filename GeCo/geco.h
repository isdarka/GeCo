#ifndef GECO_H
#define GECO_H

#include <QString>
#include <QtXml>
#include <QtSql>
#include <QSettings>
#include <QStringList>
class GeCo
{
public:
    GeCo();
private:
    QString camelCase(QString str);
};

#endif // GECO_H
