/**
 * \file   SqliteDatabase.cpp
 * \brief
 */


#include "SqliteDatabase.h"

#include <QtLib/Utils.h>


namespace qtlib
{

/**************************************************************************************************
*   public
*
**************************************************************************************************/

//-------------------------------------------------------------------------------------------------
SqliteDatabase::SqliteDatabase(
    QObject *a_parent   ///< maybe nullptr
) :
    QObject(a_parent)
{
}
//-------------------------------------------------------------------------------------------------
void
SqliteDatabase::construct(
    cSqliteConfig &a_config
)
{
    _config = &a_config;

    // qTEST(_config.isValid());

    _setup();
    _create();
    _optimize();
}
//-------------------------------------------------------------------------------------------------
QSqlDatabase &
SqliteDatabase::qDb()
{
    return _db;
}
//-------------------------------------------------------------------------------------------------
void
SqliteDatabase::fieldNames(
    cQString    &a_tableName,    ///< table name
    QStringList *a_dbFileldNames ///< field names [out]
)
{
    qTEST(_db.isValid());
    qTEST(!a_tableName.isEmpty());
    qTEST_PTR(a_dbFileldNames);

    QStringList slRv;
    QSqlQuery   qryTableInfo(_db);

    cQString sql = QString("PRAGMA table_info(%1);").arg(a_tableName);

    bool bRv = qryTableInfo.exec(sql);
    qCHECK_REF(bRv, qryTableInfo);

    while ( qryTableInfo.next() ) {
        slRv << qryTableInfo.value(1).toString();
    }

    slRv.swap(*a_dbFileldNames);
}
//-------------------------------------------------------------------------------------------------


/**************************************************************************************************
*   private
*
**************************************************************************************************/

//-------------------------------------------------------------------------------------------------
void
SqliteDatabase::_setup()
{
    bool bRv {};

    bRv = QSqlDatabase::isDriverAvailable("QSQLITE");
    qCHECK_DO(!bRv, qMSG(QSqlDatabase().lastError().text()); return);

    _db = QSqlDatabase::addDatabase("QSQLITE");
    qTEST(!qDb().driver()->hasFeature(QSqlDriver::QuerySize));

    QString databaseName;
    {
        if ( _config->dbPath.isEmpty() ) {
            databaseName = qS2QS(xl::core::Application::dbDirPath() + "/" +
                xl::core::Application::info().get().name + ".db");
        } else {
            databaseName = _config->dbPath;
        }
    }

    qDb().setDatabaseName(databaseName);
}
//-------------------------------------------------------------------------------------------------
void
SqliteDatabase::_create()
{
    bool bRv {};

    bRv = qDb().open();
    qCHECK_REF(bRv, qDb());

    Q_FOREACH (SqliteConfig::Table itTable, _config->tables) {
        QSqlQuery qryTableCreate( qDb() );

        QString  sql;
        cQString comma = ", ";

        sql += QString("CREATE TABLE IF NOT EXISTS %1 (")
                    .arg(itTable.name);

        Q_FOREACH (SqliteConfig::Table::Field itField, itTable.fields) {
            sql += QString("%1 %2%3").arg(itField.name, itField.type, comma);
        }

        sql.resize(sql.size() - comma.size());
        sql += ")";

        // qDebug() << qTRACE_VAR(sql);

        bRv = qryTableCreate.exec(sql);
        qCHECK_REF(bRv, qryTableCreate);
    } // for (_config->tables)
}
//-------------------------------------------------------------------------------------------------
void
SqliteDatabase::_optimize()
{
    // http://sqlite.org/pragma.html

    QSqlQuery("PRAGMA writable_schema = ON");

    QSqlQuery("PRAGMA page_size    = 4096 * 2");
    QSqlQuery("PRAGMA cache_size   = 16384 * 2");
    QSqlQuery("PRAGMA temp_store   = MEMORY");

    QSqlQuery("PRAGMA journal_mode = OFF");
    QSqlQuery("PRAGMA locking_mode = EXCLUSIVE");
    QSqlQuery("PRAGMA synchronous  = OFF");

    QSqlQuery("PRAGMA writable_schema = OFF");
}
//-------------------------------------------------------------------------------------------------

} // namespace qtlib
