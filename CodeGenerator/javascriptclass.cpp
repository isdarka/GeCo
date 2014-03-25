#include "javascriptclass.h"

JavaScriptClass::JavaScriptClass()
{
}

void JavaScriptClass::setDocblock(Docblock docblock)
{
    this->docblock = docblock;
}

void JavaScriptClass::setName(QString name)
{
    this->className = name;
}

void JavaScriptClass::addProperty(QString property)
{
    this->properties.append(property);
}

void JavaScriptClass::setExtend(QString name)
{
    this->extend = name;
}

QString JavaScriptClass::generate()
{
    QString string;
    string.append(this->docblock.getDocblock());
    string.append("function ").append(this->className).append("()").append("\n");
    string.append("{").append("\n");
    QString property;
    foreach (property, this->properties) {
        string.append("\t").append("this.").append(property).append(";").append("\n");
    }
    string.append("}").append("\n");

    string.append("\n");
    if(this->extend.isEmpty() || this->extend.isNull())
    {
        string.append(this->className).append(".prototype = new Bean();").append("\n");
        string.append(this->className).append(".prototype.constructor = ").append(this->className).append(";").append("\n");
    }else{
        string.append(this->className).append(".prototype = new ").append(this->extend).append("();").append("\n");
        string.append(this->className).append(".prototype.constructor = ").append(this->className).append(";").append("\n");
    }

    string.append("\n");
//    Setters
    foreach (property, this->properties)
    {
        string.append(this->className).append(".prototype.set").append(this->ucfirst(property));
        string.append(" = function(").append(property).append(")").append("\n");
        string.append("{").append("\n");
        string.append("\t").append("this.").append(property).append(" = ").append(property).append(";").append("\n");
        string.append("\t").append("return this;").append("\n");
        string.append("}").append("\n");
        string.append("\n");
    }

//    Getters
    foreach (property, this->properties)
    {
        string.append(this->className).append(".prototype.get").append(this->ucfirst(property));
        string.append(" = function()").append("\n");
        string.append("{").append("\n");
        string.append("\t").append("return this.").append(property).append(";").append("\n");
        string.append("}").append("\n");
        string.append("\n");
    }
    return string;
}

QString JavaScriptClass::ucfirst(QString str)
{
    if (str.size() < 1) {
        return "";
    }

    QStringList tokens = str.split("_");
    QList<QString>::iterator tokItr = tokens.begin();

    for (tokItr = tokens.begin(); tokItr != tokens.end(); ++tokItr) {
        (*tokItr) = (*tokItr).at(0).toUpper() + (*tokItr).mid(1);
    }
    return tokens.join("");
}
