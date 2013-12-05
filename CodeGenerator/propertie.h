#ifndef PROPERTIE_H
#define PROPERTIE_H
#include <QString>
class Propertie
{
public:
    Propertie();
    static const QString PRIVATE;
    static const QString PUBLIC;
    static const QString PROTECTED;
    void setName(QString name);
    void setVisibility(QString type);
    void setDefaultValue(QString value);
    void isConst(bool boolean);
    QString getPropertie();
private:
    QString visibility;
    QString name;
    QString defaultValue;
    bool constant;
};

#endif // PROPERTIE_H
