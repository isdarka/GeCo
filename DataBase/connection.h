#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSettings>
#include <QtSql/QSqlDatabase>
#include <QtSql>

class Connection
{
public:
    Connection(QSettings *settings);
    QSqlDatabase connect();

private:
    QSettings *settings;
    QSqlDatabase db;
};

#endif // CONNECTION_H
