#include "geco.h"

GeCo::GeCo()
{
}

QString GeCo::camelCase(QString str) {

    QString prefix;
    foreach (prefix, this->prefixes) {
        str.replace(prefix,"");
    }
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
