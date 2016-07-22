/**
 * \file   SqlTableModelEx.cpp
 * \brief
 */


#include "SqlTableModelEx.h"

#include <QtLib/Utils.h>


namespace qtlib
{

/**************************************************************************************************
*   public
*
**************************************************************************************************/

//-------------------------------------------------------------------------------------------------
SqlTableModelEx::SqlTableModelEx(
    QObject     *a_parent,
    QSqlDatabase a_db
) :
    QSqlTableModel(a_parent, a_db)
#if defined(QT_DEBUG)
    ,
    _selectCount  (0)
#endif
{
}
//-------------------------------------------------------------------------------------------------
/* virtual */
bool
SqlTableModelEx::select()
{
#if defined(QT_DEBUG)
    ++ _selectCount;

    if (_selectCount > 1) {
        qWarning() << "Warning: SqlTableModelEx::" << __FUNCTION__ << ": " << qTRACE_VAR(_selectCount);
    }
#endif

    bool bRv = false;

    Q_EMIT sig_selectProgress( rowCount() );

    bRv = QSqlTableModel::select();
    if (!bRv) {
        return false;
    }

    Q_EMIT sig_selectProgress( rowCount() );

    for ( ; ; ) {
        bRv = QSqlQueryModel::canFetchMore();
        if (!bRv) {
            break;
        }

        QSqlQueryModel::fetchMore();

        Q_EMIT sig_selectProgress( rowCount() );
    }

    return true;
}
//-------------------------------------------------------------------------------------------------
int
SqlTableModelEx::realRowCount()
{
    qTEST(!tableName().isEmpty());

    int  iRv = 0;
    bool bRv = false;

    QSqlQuery query("SELECT COUNT(*) FROM " + tableName());
    bRv = query.next();
    if (!bRv) {
        // empty table
        return 0;
    }

    iRv = query.value(0).toInt();
    qTEST(iRv >= 0);

    return iRv;
}
//-------------------------------------------------------------------------------------------------
void
SqlTableModelEx::importCsv(
    cQString               &a_filePath,
    const QVector<QString> &a_fieldNames,
    cQString               &a_csvSeparator,
    cbool                   a_isNormalize
)
{
    qTEST(!a_filePath.isEmpty());
    qTEST(!a_fieldNames.isEmpty());
    qTEST(!a_csvSeparator.isEmpty());
    qTEST_NA(a_isNormalize);

    bool bRv = false;

    // read file
    QStringList fileContent;
    {
        QFile fileCsv(a_filePath);
        bRv = fileCsv.open(QFile::ReadOnly);
        if (!bRv || !fileCsv.isReadable()) {
            return;
        }

        cQString lines = fileCsv.readAll();
        fileContent = lines.split("\n");
        qCHECK_DO(fileContent.isEmpty(), return);

        if ( fileContent.last().isEmpty() ) {
            fileContent.removeLast();
        }
    }

    // file -> DB
    for (int l = 0; l < fileContent.size(); ++ l) {
        cQStringList line = fileContent.at(l).split(a_csvSeparator);

        // targetRow
        cint targetRow = realRowCount() - 1;

        // record
        QSqlRecord record;

        for (int f = 0; f < a_fieldNames.size(); ++ f) {
            record.append( QSqlField(a_fieldNames.at(f)) );
            record.setValue( a_fieldNames.at(f), line.at(f) );
        }

        bRv = insertRecord(targetRow, record);
        qTEST(bRv);

        bRv = submitAll();
        qCHECK_PTR(bRv, this);
    }
}
//-------------------------------------------------------------------------------------------------
void
SqlTableModelEx::exportCsv(
    cQString               &a_filePath,
    const QVector<QString> &a_fieldNames,
    cQString               &a_csvSeparator,
    cbool                   a_isNormalize
)
{
    qTEST(!a_filePath.isEmpty());
    qTEST(!a_fieldNames.isEmpty());
    qTEST(!a_csvSeparator.isEmpty());
    qTEST_NA(a_isNormalize);

    // DB -> text
    QString csv;

    // DB fields -> CSV header
    {
        for (int f = 0; f < a_fieldNames.size(); ++ f) {
            csv.push_back( a_fieldNames.at(f) );

            if (f < a_fieldNames.size() - 1) {
                csv.push_back( a_csvSeparator );
            }
        }

        csv.push_back("\n");
    }

    // DB -> file
    {
        cint realRows = realRowCount();

        for (int r = 0; r < realRows; ++ r) {
            QSqlRecord rec = record(r);

            for (int f = 0; f < a_fieldNames.size(); ++ f) {
                QString fieldValue;
                {
                    QSqlField field = rec.field( a_fieldNames.at(f) );

                    switch ( field.type() ) {
                    case QVariant::String:
                        fieldValue = rec.value( a_fieldNames.at(f) ).toString();
                        break;
                    case QVariant::ByteArray:
                        fieldValue = rec.value( a_fieldNames.at(f) ).toByteArray().toBase64();
                        break;
                    default:
                        qTEST(false);
                        break;
                    }
                }

                csv.push_back(fieldValue);

                if (f < a_fieldNames.size() - 1) {
                    csv.push_back( a_csvSeparator );
                }
            }

            csv.push_back("\n");
        } // for (realRowCount)
    }

    // write to file
    {
        QFile fileCsv(a_filePath);
        bool bRv = fileCsv.open(QFile::WriteOnly | QIODevice::Text);
        qTEST(bRv);

        QTextStream stream(&fileCsv);
        stream.setCodec("UTF-8");
        stream << csv;
    }
}
//-------------------------------------------------------------------------------------------------
void
SqlTableModelEx::filter(
    cQString     &a_tableName,
    cdb_fields_t &a_fields,
    cQString     &a_join,
    cQString     &a_where,
    cQString     &a_orderBy
)
{
    qTEST(!a_tableName.isEmpty());
    qTEST(!a_fields.isEmpty());
    qTEST_NA(a_join);
    qTEST_NA(a_where);
    qTEST_NA(a_orderBy);

    cbool isSqlRelationalTableModel = (dynamic_cast<QSqlRelationalTableModel *>(this) != Q_NULLPTR);
    // qDebug() << qTRACE_VAR(isSqlRelationalTableModel);

    QString sqlStr;

    // is field values empty
    bool isAllFieldsEmpty = true;

    for (int i = 0; i < a_fields.size(); ++ i) {
        cQString ctrlValue = a_fields.at(i).second;

        qCHECK_DO(!ctrlValue.isEmpty(), isAllFieldsEmpty = false; break);
    }

    // build query
    {
        if (isAllFieldsEmpty) {
            if ( !isSqlRelationalTableModel ) {
                sqlStr = QString("SELECT * FROM %1 %2")
                                .arg(a_tableName)
                                .arg(a_join);
            }
        } else {
            if ( !isSqlRelationalTableModel ) {
                sqlStr = QString("SELECT * FROM %1 %2 WHERE")
                                .arg(a_tableName)
                                .arg(a_join);
            }

            bool isFirstNotEmptyField = true;

            for (int i = 0; i < a_fields.size(); ++ i) {
                cQString csFieldName = a_fields.at(i).first;
                cQString ctrlValue   = a_fields.at(i).second;

                qCHECK_DO(ctrlValue.isEmpty(), continue);

                // 1-st field is empty
                if (isFirstNotEmptyField) {
                    sqlStr += QString(" (%1 LIKE '%%2%')")
                                    .arg(csFieldName)
                                    .arg(ctrlValue);

                    isFirstNotEmptyField = false;
                    continue;
                }

                sqlStr += QString(" AND (%1 LIKE '%%2%')")
                                .arg(csFieldName)
                                .arg(ctrlValue);
            }
        }
    }

    // a_where
    if (!a_where.isEmpty()) {
        if (isAllFieldsEmpty) {
            sqlStr += " WHERE (" + a_where + ")";
        } else {
            sqlStr += " AND ("   + a_where + ")";
        }
    }

    // a_orderBy
    if (!a_orderBy.isEmpty()) {
        sqlStr += " " + a_orderBy + ";";
    }

    // execute query
    {
        if ( isSqlRelationalTableModel ) {
            QSqlRelationalTableModel *model = dynamic_cast<QSqlRelationalTableModel *>(this);
            model->setFilter(sqlStr);
        } else {
            QSqlQueryModel *model = dynamic_cast<QSqlQueryModel *>(this);
            model->setQuery(sqlStr);
        }

        qDebug() << sqlStr;
    }
}
//-------------------------------------------------------------------------------------------------

} // namespace qtlib
