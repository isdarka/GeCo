#ifndef JAVASCRIPTCLASS_H
#define JAVASCRIPTCLASS_H

#include "CodeGenerator/docblock.h"

class JavaScriptClass
{
public:
    JavaScriptClass();

    void setDocblock(Docblock docblock);
    void setName(QString name);
    void addProperty(QString property);
    void setExtend(QString name);
    QString ucfirst(QString str);
    QString generate();
private:
    Docblock docblock;
    QString className;
    QStringList properties;
    QString extend;
};

#endif // JAVASCRIPTCLASS_H
