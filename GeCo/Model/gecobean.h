#ifndef GECOBEAN_H
#define GECOBEAN_H

#include <QString>
#include <QStringList>

class GeCoBean
{
public:
    GeCoBean();

    void setName(QString str);
    void setTable(QString str);
    void setModule(QString str);
    void setExtend(QString str);
    void addRelation(QString str);
    void setModel(bool boolean);
    void setCrud(bool boolean);
    void setView(bool boolean);
    void setLog(bool boolean);
    void setForm(bool boolean);
    void setLogName(QString str);


    QString getName();
    QString getTable();
    QString getModule();
    QString getExtend();
    QStringList getRelations();
    bool getModel();
    bool getCrud();
    bool getView();
    bool getForm();
    bool getLog();
    QString getLogName();


private:
    QString name;
    QString table;
    QString module;
    QString extend;
    QStringList relations;
    bool form;
    bool model;
    bool crud;
    bool view;
    bool log;
    QString logName;
};

#endif // GECOBEAN_H
