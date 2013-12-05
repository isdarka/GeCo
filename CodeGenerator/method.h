#ifndef METHOD_H
#define METHOD_H
#include <QString>
#include <QStringList>
#include <CodeGenerator/docblock.h>
class Method
{
public:
    Method();
    static const QString PRIVATE;
    static const QString PUBLIC;
    static const QString PROTECTED;
    void addParam(QString paramName);
    void addParam(QString paramName, QString instance);
    void setName(QString name);
    QString generate();
    void addBody(QString body);
    void setVisibility(QString type);
    void setDocblock(Docblock docblock);
    void isStatic(bool boolean);
    bool getIsStatic();
private:
    QString name;
    QStringList params;

    QStringList paramsT;
    QStringList types;

    QStringList body;
    QString visibility;
    Docblock docblock;
    bool methodStatic = false;
};

#endif // METHOD_H

