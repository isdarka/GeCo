#include "connection.h"

Connection::Connection(QSettings *settings)
{
    this->settings = settings;
}

QSqlDatabase Connection::connect()
{
    this->db = QSqlDatabase::addDatabase("QMYSQL");
    this->settings->beginGroup("MysqlSettings");
    this->db.setHostName(this->settings->value("hostname").toString());
    this->db.setDatabaseName(this->settings->value("schema").toString());
    this->db.setUserName(this->settings->value("username").toString());
    this->db.setPassword(this->settings->value("password").toString());
    this->settings->endGroup();

    if(!this->db.isOpen())
    {
        if (!this->db.open())
            throw QString("Can't connect to database: ").append(this->db.lastError().text());
    }

    return this->db;
}
