/**
 * \file   SqliteDatabase.h
 * \brief
 */


#pragma once

#include <xLib/xLib.h>
#include "../Common.h"
#include "../Utils.h"
//-------------------------------------------------------------------------------------------------
namespace qtlib
{

struct SqliteConfig
{
    struct Table
    {
        struct Field
        {
            QString name;
            QString type;
        };

        QString        name;
        QVector<Field> fields;
    };

    QString        dbPath; /* = QString() */  ///< DB file path
    QVector<Table> tables;
};
qTYPEDEF_CONST(SqliteConfig);
//-------------------------------------------------------------------------------------------------
class SqliteDatabase :
    public QObject
{
public:
    QSqlDatabase& qDb();

    explicit      SqliteDatabase(QObject *parent /* = Q_NULLPTR */);

    void          construct(cSqliteConfig &config);


private:
    cSqliteConfig *_config;
    QSqlDatabase  _db;

    void          _setup();
    void          _create();
    void          _optimize();

    Q_DISABLE_COPY(SqliteDatabase)
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
