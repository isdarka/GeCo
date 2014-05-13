#include "method.h"
#include <QDebug>

const QString Method::PRIVATE = "private";
const QString Method::PUBLIC = "public";
const QString Method::PROTECTED = "protected";
Method::Method()
{
}

void Method::setName(QString name)
{
    this->name = name;
}

void Method::addParam(QString paramName)
{
    QStringList param;
    param.append(paramName);
    this->params.append(param);
//    this->params.append(paramName);
}

void Method::addParam(QString paramName, QString instance)
{
    QStringList param;
    param.append(paramName);
    param.append(instance);
    this->params.append(param);


//    this->paramsT.append(paramName);
//    this->types.append(instance);
}


void Method::addBody(QString body)
{
    this->body.append(body);
}

void Method::setVisibility(QString type)
{
    this->visibility = type;
}

QString Method::generate()
{
    QString method = "";
    QStringList param;

    QString body;
    method.append("\t"  + this->docblock.getDocblock("\t"));
    method.append(this->visibility);
    if(this->getIsStatic())
        method.append(" static");
    if(this->getIsFunction()){
        method.append(" function ");
        method.append(this->name);
        method.append("(");

        foreach (param, this->params)
        {
            if(param.count() == 1)
                method.append("$" + param.first() + ", ");
            else{
                method.append(param.last() + " $" + param.first() + ", ");
            }

        }

            //method.append("$" + param + ", ");

//        for(int i = 0; i<this->paramsT.count(); i++)
//        {
//            method.append(this->types.at(i) + " $" + this->paramsT.at(i) + ", ");
//        }
        //if(this->paramsT.count() > 0 || this->params.count()  > 0)
        if(this->params.count()  > 0)
            method = method.mid(0,(method.length() - 2));
        method.append(") \n");
        method.append("\t{");
        foreach (body, this->body)
            method.append("\n\t\t" + body);
        method.append("\n\t}");
    }else{
        method.append(this->name);
        method.append(" = array( \n\t");

        foreach (param, this->params)
            method.append("\t" + param.first() + ", \n\t");

        method.append("\n\t);");
    }




    return method;
}


void Method::setDocblock(Docblock docblock)
{
    this->docblock = docblock;
}

void Method::isStatic(bool boolean)
{
    this->methodStatic = boolean;
}

void Method::isFunction(bool boolean)
{
    this->methodFunction = boolean;
}

bool Method::getIsFunction()
{
    return this->methodFunction;
}

bool Method::getIsStatic()
{
    return this->methodStatic;
}
