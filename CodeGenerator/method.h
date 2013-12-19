#ifndef METHOD_H
#define METHOD_H
#include <QString>
#include <QStringList>
#include <CodeGenerator/docblock.h>
#include <QVector>
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
    void isFunction(bool boolean);
    bool getIsFunction();
private:
    QString name;
//    QStringList params;

//    QStringList paramsT;
//    QStringList types;

    QStringList body;
    QString visibility;
    Docblock docblock;
    QVector<QStringList> params;
    bool methodStatic = false;
    bool methodFunction = true;
};

#endif // METHOD_H

