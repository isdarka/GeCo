#include "propertie.h"
#include <QDebug>
const QString Propertie::PRIVATE = "private";
const QString Propertie::PUBLIC = "public";
const QString Propertie::PROTECTED = "protected";
Propertie::Propertie()
{
}

void Propertie::setName(QString name)
{
    this->name = name;
}

void Propertie::setDefaultValue(QString value)
{
    this->defaultValue = value;
}

void Propertie::setVisibility(QString type)
{
    this->visibility = type;
}

void Propertie::isConst(bool boolean)
{
    this->constant = boolean;
}

QString Propertie::getPropertie()
{
    try{
        QString propertie = "";
        if(this->name.isEmpty())
            throw QString("Propertie name empty");
        if(this->constant){
            //const bat = 'foobarbazbat';
            if(this->defaultValue.isEmpty())
                throw QString("Default value can't be empty for constant");
            propertie.append("const " + this->name + " = ");
            propertie.append("'" + this->defaultValue+ "';");
        }else{
            //public $baz = 'bat';
            if(this->visibility.isEmpty())
                throw QString("Visibility can't be empty");
            propertie.append(this->visibility + " $" + this->name);
            if(this->defaultValue.isEmpty())
                propertie.append(";");
            else
                propertie.append(" = '" + this->defaultValue+ "';");

        }

        return propertie;
    }catch(QString e){
        qDebug() << e;
    }
}
