#ifndef TABLECATALOG_H
#define TABLECATALOG_H
#include <QVector>
#include <QString>
#include "DataBase/Model/columnbean.h"

class TableCatalog
{
public:
    TableCatalog();
    void test();
    QVector<ColumnBean> getColumnsByTable(QString tableName);
private:

    QVector<ColumnBean> columns;
};

#endif // TABLECATALOG_H
