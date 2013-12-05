#include "phpclass.h"
#include <QDebug>
PhpClass::PhpClass()
{
    this->extends.null;
    this->className.null;
    this->implement.null;

}

void PhpClass::setClassName(QString str)
{
    this->className = str;
}

void PhpClass::setExtends(QString extend)
{
    this->extends = extend;
}

void PhpClass::setImplement(QString implement)
{
    this->implement = implement;
}


QString PhpClass::getClassName()
{
    return this->className;
}

QString PhpClass::getExtends()
{
    return this->extends;
}

QString PhpClass::getImplement()
{
    return this->implement;
}

bool PhpClass::isExtends()
{
    if(this->getExtends().isEmpty())
        return false;
    else if(this->getExtends().isNull())
        return false;
    else
        return true;
}

bool PhpClass::isImplement()
{
    if(this->getImplement().isEmpty())
        return false;
    else if(this->getImplement().isNull())
        return false;
    else
        return true;
}

QString PhpClass::generate()
{
    QString code = "";
    Method method;
    Propertie propertie;
    code = "<?php";
    code.append("\n");

    //if(this->docblock == NULL)

        code.append(this->docblock.getDocblock());
    code.append("class ");
    code.append(this->getClassName());
    if(this->isExtends())
    {
        code.append(" extends ");
        code.append(this->getExtends());
    }
    if(this->isImplement())
    {
        code.append(" implements ");
        code.append(this->getImplement());
    }
    code.append("\n{\n");

    //SetProperties
    foreach (propertie, this->properties) {
        code.append("\n\t" + propertie.getPropertie() + "\n");
    }

    //SetMethods
    foreach (method, this->methods) {
        code.append("\n\t" + method.generate());
    }
    code.append("\n}");
    //code += '?>';
    return code;
}


void PhpClass::addMethod(Method method)
{
    this->methods.append(method);
}


void PhpClass::addPropertie(Propertie propertie)
{
    this->properties.append(propertie);
}


void PhpClass::setDocblock(Docblock docblock)
{
    this->docblock = docblock;
}
