/**
 * \file   SqlRelationalTableModelEx.cpp
 * \brief
 */


#include "SqlRelationalTableModelEx.h"

#include <QtLib/Utils.h>


namespace qtlib
{

/**************************************************************************************************
*   public
*
**************************************************************************************************/

//-------------------------------------------------------------------------------------------------
SqlRelationalTableModelEx::SqlRelationalTableModelEx(
    QObject     *a_parent,
    QSqlDatabase a_db
) :
    QSqlRelationalTableModel(a_parent, a_db)
{
}
//-------------------------------------------------------------------------------------------------
/* virtual */
bool
SqlRelationalTableModelEx::select()
{
#if defined(QT_DEBUG)
    ++ _selectCount;

    if (_selectCount > 1) {
        qWarning() << "Warning: SqlRelationalTableModelEx::" << __FUNCTION__ << ": " << qTRACE_VAR(_selectCount);
    }
#endif

    bool bRv = false;

    Q_EMIT sig_selectProgress( rowCount() );

    bRv = QSqlRelationalTableModel::select();
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
SqlRelationalTableModelEx::realRowCount()
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
SqlRelationalTableModelEx::importCsv(
    cQString               &a_filePath,     ///< CSV path, if empty - import from clipboard
    const QVector<QString> &a_fieldNames,   ///< DB table columns
    cQString               &a_csvSeparator, ///< CSV separator
    cbool                   a_isNormalize,  ///< normalize filed names/values
    QString                *out_infoMsg     ///< [out] info message (statistic)
)
{
    qTEST_NA(a_filePath);
    qTEST(!a_fieldNames.isEmpty());
    qTEST(!a_csvSeparator.isEmpty());
    qTEST_NA(a_isNormalize);
    qTEST_NA(out_infoMsg);

    bool bRv {};

    struct Info
    {
        int wordsAll {};
        int wordsDuplicates {};
        int wordsSkip {};
        int wordsDone {};
    };

    Info info;

    // read file
    QStringList csvContent;
    {
        QString lines;

        if ( !a_filePath.isEmpty() ) {
            QFile fileCsv(a_filePath);
            bRv = fileCsv.open(QFile::ReadOnly);
            if (!bRv || !fileCsv.isReadable()) {
                return;
            }

            lines = fileCsv.readAll();
        } else {
            lines = QApplication::clipboard()->text();
        }

        csvContent = lines.split("\n");
        qCHECK_DO(csvContent.isEmpty(), return);

        if ( csvContent.last().isEmpty() ) {
            csvContent.removeLast();
        }

        info.wordsAll        = csvContent.size();
        info.wordsDuplicates = csvContent.removeDuplicates();
    }

    // file -> DB
    for (int l = 0; l < csvContent.size(); ++ l) {
        QStringList lineColumns;
        {
            cQString rawLine = csvContent.at(l);

            lineColumns = rawLine.split(a_csvSeparator);

            cint csvSeparatorsNum = rawLine.count(a_csvSeparator);
            if (csvSeparatorsNum == a_fieldNames.size()) {
                // line with all fields
                lineColumns = rawLine.split(a_csvSeparator);
            } else {
                // line with term field only
                lineColumns.append( rawLine.trimmed() );
            }
        }

        // targetRow
        cint targetRow = realRowCount() - 1;

        // record
        QSqlRecord record;

        for (int f = 0; f < a_fieldNames.size(); ++ f) {
            QString fieldName = a_fieldNames.at(f);

            QString fieldValue;
            {
                if      (lineColumns.size() == a_fieldNames.size()) {
                    fieldValue = lineColumns.at(f);
                }
                else if (f == 0) {
                    fieldValue = lineColumns.at(f);
                }
                else {
                    fieldValue = "";
                }
            }

            // normalize
            if (a_isNormalize) {
                _csvNormalize(&fieldName, &fieldValue);
            }

            record.append( QSqlField(fieldName) );
            record.setValue(fieldName, fieldValue);
        }

        bRv = insertRecord(targetRow, record);
        qTEST(bRv);

        bRv = submitAll();
        if (!bRv &&
            lastError().text().contains("failed", Qt::CaseInsensitive))
        {
            // lastError(): QSqlError("19", "Unable to fetch row", "UNIQUE constraint failed: t_main.f_main_term")
            // qDebug() << qTRACE_VAR(lastError().text());
            ++ info.wordsSkip;

            continue;
        }

        qCHECK_PTR(bRv, this);

        ++ info.wordsDone;
    } // for (csvContent)

    // [out]
    if (out_infoMsg != nullptr) {
        *out_infoMsg = QString("Words: All - %1, Duplicates - %2, Skip - %3, Done - %4")
                        .arg(info.wordsAll)
                        .arg(info.wordsDuplicates)
                        .arg(info.wordsSkip)
                        .arg(info.wordsDone);
    }
}
//-------------------------------------------------------------------------------------------------
void
SqlRelationalTableModelEx::importCsvClipboard(
    const QVector<QString> &a_fieldNames,   ///< DB table columns
    cQString               &a_csvSeparator, ///< CSV separator
    cbool                   a_isNormalize,  ///< normalize filed names/values
    QString                *out_infoMsg     ///< [out] info message (statistic)
)
{
    importCsv("", a_fieldNames, a_csvSeparator, a_isNormalize, out_infoMsg);
}
//-------------------------------------------------------------------------------------------------
void
SqlRelationalTableModelEx::exportCsv(
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
    QString csvContent;

    // DB fields -> CSV header
    {
        for (int f = 0; f < a_fieldNames.size(); ++ f) {
            QString fieldName = a_fieldNames.at(f);

            // normalize
            if (a_isNormalize) {
                _csvNormalize(&fieldName, nullptr);
            }

            csvContent.push_back(fieldName);

            if (f < a_fieldNames.size() - 1) {
                csvContent.push_back( a_csvSeparator );
            }
        }

        csvContent.push_back("\n");
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

                // normalize
                if (a_isNormalize) {
                    _csvNormalize(nullptr, &fieldValue);
                }

                csvContent.push_back(fieldValue);

                if (f < a_fieldNames.size() - 1) {
                    csvContent.push_back( a_csvSeparator );
                }
            }

            csvContent.push_back("\n");
        } // for (realRowCount)
    }

    // write to file
    {
        QFile fileCsv(a_filePath);
        bool bRv = fileCsv.open(QFile::WriteOnly | QIODevice::Text);
        qTEST(bRv);

        QTextStream stream(&fileCsv);
        stream.setCodec("UTF-8");
        stream << csvContent;
    }
}
//-------------------------------------------------------------------------------------------------
void
SqlRelationalTableModelEx::filter(
    cdb_fields_t &a_fields,
    cQString     &a_join,
    cQString     &a_where,
    cQString     &a_orderBy
)
{
    qTEST(!a_fields.isEmpty());
    qTEST_NA(a_join);
    qTEST_NA(a_where);
    qTEST_NA(a_orderBy);

    cbool isSqlRelationalTableModel = (dynamic_cast<QSqlRelationalTableModel *>(this) != nullptr);
    qDebug() << qTRACE_VAR(isSqlRelationalTableModel);

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
                                .arg( tableName() )
                                .arg(a_join);
            }
        } else {
            if ( !isSqlRelationalTableModel ) {
                sqlStr = QString("SELECT * FROM %1 %2 WHERE")
                                .arg( tableName() )
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


/**************************************************************************************************
*   private
*
**************************************************************************************************/

//-------------------------------------------------------------------------------------------------
void
SqlRelationalTableModelEx::_csvNormalize(
    QString *a_fieldName,
    QString *a_fieldValue
)
{
    qCHECK_DO(a_fieldName == nullptr && a_fieldValue == nullptr, return);

    // trim
    {
        if (a_fieldName != nullptr) {
            *a_fieldName  = a_fieldName->trimmed();
        }

        if (a_fieldValue != nullptr) {
            *a_fieldValue = a_fieldValue->trimmed();
        }
    }
}
//-------------------------------------------------------------------------------------------------

} // namespace qtlib
