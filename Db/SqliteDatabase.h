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

    explicit      SqliteDatabase(QObject *parent /* = nullptr */);

    void          construct(cSqliteConfig &config);
    QSqlDatabase& qDb();

    void          fieldNames(cQString &tableName, QStringList *dbFileldNames);
        ///< DB filed names

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
