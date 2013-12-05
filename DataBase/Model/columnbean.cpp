#include "columnbean.h"

ColumnBean::ColumnBean()
{
}

void ColumnBean::setField(QString str)
{
    this->field = str;
}

void ColumnBean::setType(QString str)
{
    this->type = str;
}

void ColumnBean::setNull(bool boolean)
{
    this->isNull = boolean;
}

void ColumnBean::setKey(QString str)
{
     this->key = str;
}

void ColumnBean::setDefault(QString str)
{
    this->defaultValue = str;
}

void ColumnBean::setExtra(QString str)
{
    this->extra = str;
}

QString ColumnBean::getField()
{
    return this->field;
}

QString ColumnBean::getType()
{
    return this->type;
}

bool ColumnBean::getNull()
{
    return this->isNull;
}

QString ColumnBean::getKey()
{
    return this->key;
}

QString ColumnBean::getDefault()
{
    return this->defaultValue;
}

QString ColumnBean::getExtra()
{
    return this->extra;
}

/**
 * DATE	YYYY-MM-DD.
 * DATETIME	YYYY-MM-DD HH:MM:SS.
 * TIMESTAMP	YYYYMMDDHHMMSS.
 * @return boolean
 */
bool ColumnBean::isDate()
{
    if(this->type.toUpper() == "DATE")
        return true;
    if(this->type.toUpper() == "DATETIME")
        return true;
    if(this->type.toUpper() == "TIMESTAMP")
        return true;

    return false;
}

/**
 * TIME	HH:MM:SS.
 * @return boolean
 */
bool ColumnBean::isTime()
{
    if(this->type.toUpper() == "TIME")
        return true;

    return false;
}

/**
 * 	CHAR( )	A fixed section from 0 to 255 characters long.
 *	VARCHAR( )	A variable section from 0 to 255 characters long.
 *	TINYTEXT	A string with a maximum length of 255 characters.
 *	TEXT	A string with a maximum length of 65535 characters.
 *	BLOB	A string with a maximum length of 65535 characters.
 *	MEDIUMTEXT	A string with a maximum length of 16777215 characters.
 *	MEDIUMBLOB	A string with a maximum length of 16777215 characters.
 *	LONGTEXT	A string with a maximum length of 4294967295 characters.
 *	LONGBLOB	A string with a maximum length of 4294967295 characters.
 */
bool ColumnBean::isText()
{
    if(this->type.toUpper().mid(0,4) == "CHAR")
        return true;
    if(this->type.toUpper().mid(0,7) == "VARCHAR")
        return true;
    if(this->type.toUpper() == "TINYTEXT")
        return true;
    if(this->type.toUpper() == "TEXT")
        return true;
    if(this->type.toUpper() == "BLOB")
        return true;
    if(this->type.toUpper() == "MEDIUMTEXT")
        return true;
    if(this->type.toUpper() == "MEDIUMBLOB")
        return true;
    if(this->type.toUpper() == "LONGTEXT")
        return true;
    if(this->type.toUpper() == "LONGBLOB")
        return true;

    return false;
}

bool ColumnBean::isLongText()
{
    if(this->type.toUpper() == "TEXT")
        return true;
    if(this->type.toUpper() == "BLOB")
        return true;
    if(this->type.toUpper() == "MEDIUMTEXT")
        return true;
    if(this->type.toUpper() == "MEDIUMBLOB")
        return true;
    if(this->type.toUpper() == "LONGTEXT")
        return true;
    if(this->type.toUpper() == "LONGBLOB")
        return true;

    return false;
}

/**
 *  TINYINT( )	-128 to 127 normal 0 to 255 UNSIGNED.
 *	SMALLINT( )	-32768 to 32767 normal 0 to 65535 UNSIGNED.
 *	MEDIUMINT( )	-8388608 to 8388607 normal 0 to 16777215 UNSIGNED.
 *	INT( )	-2147483648 to 2147483647 normal 0 to 4294967295 UNSIGNED.
 *	BIGINT( )	-9223372036854775808 to 9223372036854775807 normal 0 to 18446744073709551615 UNSIGNED.
 *	FLOAT	A small number with a floating decimal point.
 *	DOUBLE( , )	A large number with a floating decimal point.
 *	DECIMAL( , )	A DOUBLE stored as a string , allowing for a fixed decimal point.
 */
bool ColumnBean::isNumber()
{
    if(this->type.toUpper().mid(0,7) == "TINYINT")
        return true;
    if(this->type.toUpper().mid(0,5) == "SMALLINT")
        return true;
    if(this->type.toUpper().mid(0,9) == "MEDIUMINT")
        return true;
    if(this->type.toUpper().mid(0,3) == "INT")
        return true;
    if(this->type.toUpper().mid(0,6) == "BIGINT")
        return true;
    if(this->type.toUpper().mid(0,5) == "FLOAT")
        return true;
    if(this->type.toUpper().mid(0,6) == "DOUBLE")
        return true;
    if(this->type.toUpper().mid(0,7) == "DECIMAL")
        return true;
    return false;
}


bool ColumnBean::isPrimaryKey()
{
    if(this->key.toUpper() == "PRI")
        return true;
    return false;
}

bool ColumnBean::isForeingKey()
{
    if(this->key.toUpper() == "MUL")
        return true;
    return false;
}

bool ColumnBean::isUnique()
{
    if(this->key.toUpper() == "UNI")
        return true;
    return false;
}

bool ColumnBean::isPkAutoIncrement()
{
    if(this->key.toUpper() == "PRI" && this->extra.toLower() == "auto_increment")
        return true;
    return false;
}
