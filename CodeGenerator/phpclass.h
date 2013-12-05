#ifndef PHPCLASS_H
#define PHPCLASS_H
#include <QString>
#include <CodeGenerator/method.h>
#include <QVector>
#include <CodeGenerator/propertie.h>
#include <CodeGenerator/docblock.h>
class PhpClass
{
public:
    PhpClass();
    void setClassName(QString str);
    void setExtends(QString extend);
    void setImplement(QString implement);
    //Getters
    QString getClassName();
    QString getExtends();
    QString getImplement();

    QString generate();
    bool isExtends();
    bool isImplement();
    void addMethod(Method method);
    void addPropertie(Propertie propertie);
    void setDocblock(Docblock docblock);
private:
    QString className;
    QString extends;
    QString implement;
    QVector<Method> methods;
    QVector<Propertie> properties;
    Docblock docblock;
};

#endif // PHPCLASS_H
