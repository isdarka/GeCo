#ifndef DOCBLOCK_H
#define DOCBLOCK_H
#include <QString>
#include <QStringList>
#include <QVector>
class Docblock
{
public:
    Docblock();

    static const QString AUTOR;
    static const QString CATEGORY;
    static const QString VERSION;
    static const QString CREATED;
    static const QString LICENSE;
    static const QString PACKAGE;
    static const QString ABSTRACT;
    static const QString DEPRECATED;
    static const QString EXAMPLE;
    static const QString RETURN;
    static const QString THROWS;
    static const QString INT;
    static const QString FLOAT;
    static const QString STRING;
    static const QString ARRAY;
    static const QString UNKNOWN;


    void setShortDescription(QString text);
    void setLongDescription(QString text);
    void addTag(QString type, QString value);
    QString getDocblock();
    QString getDocblock(QString spaces);
private:
    QString shortD;
    QString longD;
    QVector<QString> tagsTypes;
    QVector<QString> tagsValues;

};

#endif // DOCBLOCK_H
