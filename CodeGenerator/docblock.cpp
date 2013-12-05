#include "docblock.h"

const QString Docblock::AUTOR="autor";
const QString Docblock::CATEGORY="category";
const QString Docblock::VERSION="version";
const QString Docblock::CREATED="created";
const QString Docblock::LICENSE="license";
const QString Docblock::PACKAGE="package";
const QString Docblock::ABSTRACT="abstract";
const QString Docblock::DEPRECATED="deprecated";
const QString Docblock::EXAMPLE="example";
const QString Docblock::RETURN="return";
const QString Docblock::THROWS="throws";
const QString Docblock::INT ="int";
const QString Docblock::FLOAT = "float";
const QString Docblock::STRING ="string";
const QString Docblock::ARRAY ="array";
const QString Docblock::UNKNOWN ="unknown";

Docblock::Docblock()
{
}


void Docblock::setShortDescription(QString text)
{
    this->shortD = text;
}

void Docblock::setLongDescription(QString text)
{
    this->longD = text;
}

void Docblock::addTag(QString type, QString value)
{
    this->tagsTypes.append(type);
    this->tagsValues.append(value);
}

QString Docblock::getDocblock()
{
    QString dock = "";
    dock.append("\n/**");
    dock.append("\n *");
    QString tag;
    if(!this->shortD.isEmpty()){
        dock.append("\n * " + this->shortD);
        dock.append("\n * ");
    }
    if(!this->longD.isEmpty()){
        dock.append("\n * " + this->longD);
        dock.append("\n * ");
    }
    for(int i = 0; i < this->tagsTypes.count(); i++)
    {
        dock.append("\n * @" + this->tagsTypes.at(i) + " " + this->tagsValues.at(i));
    }
    dock.append("\n */");
    dock.append("\n");

    return dock;

}


QString Docblock::getDocblock(QString spaces = "")
{
    QString dock = "";
    dock.append("\n " + spaces +"/**");
    dock.append("\n " + spaces +" *");
    QString tag;
    if(!this->shortD.isEmpty()){
        dock.append("\n " + spaces +" * " + this->shortD);
        dock.append("\n " + spaces +" *");
    }
    if(!this->longD.isEmpty()){
        dock.append("\n " + spaces +" * " + this->longD);
        dock.append("\n " + spaces +" *");
    }
    for(int i = 0; i < this->tagsTypes.count(); i++)
    {
        dock.append("\n " + spaces +" * @" + this->tagsTypes.at(i) + " " + this->tagsValues.at(i));
    }
    dock.append("\n " + spaces +" */");
    dock.append("\n" + spaces);

    return dock;

}
