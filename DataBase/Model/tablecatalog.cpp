#include "tablecatalog.h"
#include <QtSql>
#include <QDebug>
TableCatalog::TableCatalog()
{
}


void TableCatalog::test()
{
    QSqlQuery query;

    query.exec("SHOW TABLES");

//    while(query.next()) {
//        qDebug()<< query.value(0).toString();
//    }
}

QVector<ColumnBean> TableCatalog::getColumnsByTable(QString tableName)
{
    QSqlQuery query;

    query.exec("DESCRIBE " + tableName);

    while(query.next()) {
        ColumnBean bean;

        bean.setField(query.value(0).toString());
        bean.setType(query.value(1).toString());
        //qDebug() << query.value(0).toString() + " -- " + query.value(2).toString();
        if(query.value(2).toString() == "YES")
            bean.setNull(true);
        else
            bean.setNull(false);
        bean.setKey(query.value(3).toString());
        bean.setDefault(query.value(4).toString());
        bean.setExtra(query.value(5).toString());
        this->columns.append(bean);
    }

    return this->columns;
}
