#ifndef COLUMNBEAN_H
#define COLUMNBEAN_H
#include <QString>
class ColumnBean
{
public:
    ColumnBean();

    void setField(QString str);
    void setType(QString str);
    void setNull(bool boolean);
    void setKey(QString str);
    void setDefault(QString str);
    void setExtra(QString str);

    QString getField();
    QString getType();
    bool getNull();
    QString getKey();
    QString getDefault();
    QString getExtra();

    bool isDate();
    bool isTime();
    bool isText();
    bool isLongText();
    bool isNumber();
    bool isPrimaryKey();
    bool isForeingKey();
    bool isUnique();
    bool isPkAutoIncrement();
private:
    QString field;
    QString type;
    bool isNull;
    QString key;
    QString defaultValue;
    QString extra;

};

#endif // COLUMNBEAN_H
