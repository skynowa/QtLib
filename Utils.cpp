/**
 * \file   Utils.cpp
 * \brief
 */


#include "Utils.h"

#include "Application.h"

#if defined(Q_OS_WIN)
    #include <windows.h>
#endif

#include <QTextStream>


namespace qtlib
{

/**************************************************************************************************
*   private
*
**************************************************************************************************/

//-------------------------------------------------------------------------------------------------
Utils::Utils()
{
}
//-------------------------------------------------------------------------------------------------


/**************************************************************************************************
*   public
*
**************************************************************************************************/


/**************************************************************************************************
*   GUI
*
**************************************************************************************************/

//-------------------------------------------------------------------------------------------------
/* static */
void
Utils::widgetAlignCenter(
    QWidget *a_widget
)
{
    qTEST_PTR(a_widget);

    QRect rect = QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, a_widget->size(),
        qApp->desktop()->availableGeometry());

    a_widget->setGeometry(rect);
}
//-------------------------------------------------------------------------------------------------
/* static */
void
Utils::widgetAlignTopCenter(
    QWidget *a_widget
)
{
    qTEST_PTR(a_widget);

    QDesktopWidget *desktop = QApplication::desktop();
    qTEST_PTR(desktop);

    cint x = (desktop->width() - a_widget->width()) / 2;
    cint y = 0;

    a_widget->setGeometry(x, y, a_widget->width(), a_widget->height());
}
//-------------------------------------------------------------------------------------------------
void
Utils::widgetAlwaysOnTop(
    QWidget *a_widget,
    cbool   &a_isChecked
)
{
    qTEST_PTR(a_widget);
    // a_isChecked - n/a

    Qt::WindowFlags flags = a_widget->windowFlags();
    if (a_isChecked) {
        flags |= (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    } else {
        flags &= ~(Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    }

    a_widget->setWindowFlags(flags);
}
//-------------------------------------------------------------------------------------------------


/**************************************************************************************************
*   DB
*
**************************************************************************************************/

//-------------------------------------------------------------------------------------------------
/* static */
int
Utils::sqlTableModelRowCount(
    QSqlTableModel *a_model
)
{
    qTEST_PTR(a_model);
    qTEST(!a_model->tableName().isEmpty());

    qCHECK_RET(a_model == Q_NULLPTR, 0);

    int  iRv = 0;
    bool bRv = false;

    QSqlQuery query("SELECT COUNT(*) FROM " + a_model->tableName());
    bRv = query.next();
    if (!bRv) {
        qTEST(false);
        return 0;
    }

    iRv = query.value(0).toInt();
    qTEST(iRv >= 0);

    return iRv;
}
//-------------------------------------------------------------------------------------------------
/* static */
void
Utils::importCsv(
    cQString               &a_filePath,
    QSqlTableModel         *a_sqlTableModel,
    const QVector<QString> &a_fieldNames,
    cQString               &a_columnSeparator
)
{
    qTEST(!a_filePath.isEmpty());
    qTEST_PTR(a_sqlTableModel);
    qTEST(!a_fieldNames.isEmpty());
    qTEST(!a_columnSeparator.isEmpty());

    bool bRv = false;

    // read file
    QStringList file;
    {
        QFile fileCSV(a_filePath);

        bRv = fileCSV.open(QFile::ReadOnly);
        qTEST(bRv);

        cQString lines = fileCSV.readAll();
        file = lines.split("\n");

        fileCSV.close();

        qCHECK_DO(file.isEmpty(), return);

        if (file.last().isEmpty()) {
            file.removeLast();
        }
    }

    // file -> DB
    for (int l = 0; l < file.size(); ++ l) {
        cQStringList line = file.at(l).split(a_columnSeparator);

        // targetRow
        cint targetRow = Utils::sqlTableModelRowCount(a_sqlTableModel) - 1;

        // record
        QSqlRecord record;

        for (int f = 0; f < a_fieldNames.size(); ++ f) {
            record.append(QSqlField(a_fieldNames.at(f)));
            record.setValue(a_fieldNames.at(f), line.at(f));
        }

        bRv = a_sqlTableModel->insertRecord(targetRow, record);
        qTEST(bRv);

        bRv = a_sqlTableModel->submitAll();
        qCHECK_PTR(bRv, a_sqlTableModel);
    }
}
//-------------------------------------------------------------------------------------------------
/* static */
void
Utils::exportCsv(
    cQString               &a_filePath,
    QSqlTableModel         *a_sqlTableModel,
    const QVector<QString> &a_fieldNames,
    cQString               &a_columnSeparator
)
{
    qTEST(!a_filePath.isEmpty());
    qTEST_PTR(a_sqlTableModel);
    qTEST(!a_fieldNames.isEmpty());
    qTEST(!a_columnSeparator.isEmpty());

    // DB -> text
    QString csv;

    // DB fields -> CSV header
    {
        for (int f = 0; f < a_fieldNames.size(); ++ f) {
            csv.push_back( a_fieldNames.at(f) );

            if (f < a_fieldNames.size() - 1) {
                csv.push_back( a_columnSeparator );
            }
        }

        csv.push_back("\n");
    }

    // DB -> file
    {
        cint realRowCount = Utils::sqlTableModelRowCount(a_sqlTableModel);

        for (int r = 0; r < realRowCount; ++ r) {
            for (int f = 0; f < a_fieldNames.size(); ++ f) {
                csv.push_back( a_sqlTableModel->record(r).value( a_fieldNames.at(f) ).toString() );

                if (f < a_fieldNames.size() - 1) {
                    csv.push_back( a_columnSeparator );
                }
            }

            csv.push_back("\n");
        }
    }

    // write to file
    {
        QFile fileCSV(a_filePath);

        bool bRv = fileCSV.open(QFile::WriteOnly | QIODevice::Text);
        qTEST(bRv);

        QTextStream stream(&fileCSV);

        stream.setCodec("UTF-8");
        stream << csv;
    }
}
//-------------------------------------------------------------------------------------------------
/* static */
void
Utils::dbFilter(
    QSqlQueryModel *a_sqlQueryModel,
    cQString       &a_tableName,
    cdb_fields_t   &a_fields,
    cQString       &a_sqlStrJoin,
    cQString       &a_sqlStrWhere,
    cQString       &a_sqlStrOrderBy
)
{
    qTEST_PTR(a_sqlQueryModel);
    qTEST(!a_tableName.isEmpty());
    qTEST(!a_fields.isEmpty());
    // a_sqlStrJoin - n/a
    // a_sqlStrWhere - n/a
    // a_sqlStrOrderBy - n/a

    cbool isSqlRelationalTableModel
        = (dynamic_cast<QSqlRelationalTableModel *>(a_sqlQueryModel) != Q_NULLPTR);
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
                                .arg(a_tableName)
                                .arg(a_sqlStrJoin);
            }
        } else {
            if ( !isSqlRelationalTableModel ) {
                sqlStr = QString("SELECT * FROM %1 %2 WHERE")
                                .arg(a_tableName)
                                .arg(a_sqlStrJoin);
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

    // a_sqlStrWhere
    if (!a_sqlStrWhere.isEmpty()) {
        if (isAllFieldsEmpty) {
            sqlStr += " WHERE (" + a_sqlStrWhere + ")";
        } else {
            sqlStr += " AND ("   + a_sqlStrWhere + ")";
        }
    }

    // a_sqlStrOrderBy
    if (!a_sqlStrOrderBy.isEmpty()) {
        sqlStr += " " + a_sqlStrOrderBy + ";";
    }

    // execute query
    {
        if ( !isSqlRelationalTableModel ) {
            a_sqlQueryModel->setQuery(sqlStr);
        } else {
            QSqlRelationalTableModel *model = dynamic_cast<QSqlRelationalTableModel *>(a_sqlQueryModel);
            model->setFilter(sqlStr);
        }

        qDebug() << sqlStr;
    }
}
//-------------------------------------------------------------------------------------------------
void
Utils::dbFieldNames(
    const QSqlDatabase &a_db,           ///< database
    cQString           &a_tableName,    ///< table name
    QStringList        *a_dbFileldNames ///< field names [out]
)
{
    qTEST(a_db.isValid());
    qTEST(!a_tableName.isEmpty());
    qTEST_PTR(a_dbFileldNames);

    QStringList slRv;
    QSqlQuery   qryTableInfo(a_db);

    cQString sql =
        "pragma table_info(" + a_tableName + ");";

    bool bRv = qryTableInfo.exec(sql);
    qCHECK_REF(bRv, qryTableInfo);

    while (qryTableInfo.next()) {
        slRv << qryTableInfo.value(1).toString();
    }

    slRv.swap(*a_dbFileldNames);
}
//-------------------------------------------------------------------------------------------------


/**************************************************************************************************
*   web
*
**************************************************************************************************/


/**************************************************************************************************
*   grafics
*
**************************************************************************************************/

//-------------------------------------------------------------------------------------------------
/* static */
void
Utils::imageConvert(
    cQString    &a_filePathIn,
    QByteArray  *a_photo,
    cQByteArray &a_format /* = "JPEG" */
)
{
    qTEST(!a_filePathIn.isEmpty());
    qTEST_PTR(a_photo);
    qTEST(!a_format.isEmpty());

    QBuffer      buffer(a_photo);
    QImageWriter writer(&buffer, a_format);
    QImage       image (a_filePathIn);

    /// writer.setCompression(9);

    bool bRv = writer.write(image);
    qTEST(bRv);

    buffer.close();
}
//-------------------------------------------------------------------------------------------------


/**************************************************************************************************
*   converters
*
**************************************************************************************************/

//-------------------------------------------------------------------------------------------------
/* static */
std::wstring
Utils::toStdWString(
    cQString &a_str
)
{
    // a_str - n/a

#if defined(_MSC_VER)
    return std::wstring(reinterpret_cast<const wchar_t *>( a_str.utf16() ));
#else
    return a_str.toStdWString();
#endif
}
//-------------------------------------------------------------------------------------------------
/* static */
QString
Utils::fromStdWString(
    const std::wstring &a_str
)
{
    // a_str - n/a

#if defined(_MSC_VER)
    return QString::fromUtf16(reinterpret_cast<const ushort *>( a_str.c_str() ));
#else
    return QString::fromStdWString(a_str);
#endif
}
//-------------------------------------------------------------------------------------------------
/* static */
QString
Utils::formatBytes(
    culonglong &a_bytes
)
{
    // a_bytes - n/a

    QString sRv;

    culonglong tb   = 1024ULL * 1024ULL * 1024ULL * 1024ULL;
    culonglong gb   = 1024ULL * 1024ULL * 1024ULL;
    culonglong mb   = 1024ULL * 1024ULL;
    culonglong kb   = 1024ULL;
    culonglong byte = 1ULL;

    if      (a_bytes / tb   > 0ULL) {
        sRv.sprintf("%.2f TB", static_cast<double>(a_bytes) / static_cast<double>(tb));
    }
    else if (a_bytes / gb   > 0ULL) {
        sRv.sprintf("%.2f GB", static_cast<double>(a_bytes) / static_cast<double>(gb));
    }
    else if (a_bytes / mb   > 0ULL) {
        sRv.sprintf("%.2f MB", static_cast<double>(a_bytes) / static_cast<double>(mb));
    }
    else if (a_bytes / kb   > 0ULL) {
        sRv.sprintf("%.2f KB", static_cast<double>(a_bytes) / static_cast<double>(kb));
    }
    else if (a_bytes / byte > 0ULL) {
        sRv.sprintf("%.2f Byte(s)", static_cast<double>(a_bytes) / static_cast<double>(byte));
    }
    else {
        sRv.sprintf("%.2f Bit(s)", static_cast<double>(a_bytes));
    }

    return sRv;
}
//-------------------------------------------------------------------------------------------------


/**************************************************************************************************
*   debug
*
**************************************************************************************************/

//-------------------------------------------------------------------------------------------------
void
Utils::debugTest(
    cQString &a_expression,
    cQString &a_filePath,
    culong   &a_fileLine,
    cQString &a_functionName,
    cqint64  &a_logSizeMaxBytes
)
{
    cQString msg = QString(
        "\n-------------------- qTEST ----------------------\n"
        " Expression: %1\n"
        " File:       %2\n"
        " Line:       %3\n"
        " Function:   %4\n"
        " Date:       %5\n"
        "--------------------------------------------------\n")
            .arg(a_expression)
            .arg(a_filePath)
            .arg(a_fileLine)
            .arg(a_functionName)
            .arg( QDateTime::currentDateTime().toString() );

#if Q_OS_ANDROID
    Q_UNUSED(a_logSizeMaxBytes);
#else
    // write to file
    {
        bool bRv = false;

        cQString logPath = QApplication::applicationDirPath() + QDir::separator() +
            QApplication::applicationName() + ".log";

        QFile log(logPath);
        bRv = log.open(QFile::Append | QIODevice::Text);
        if (!bRv) {
            qWarning()
                << "Utils::debugTest: "
                << log.errorString() << ", "
                << qTRACE_VAR(logPath);

            return;
        }

        // truncate log file
        if (log.size() > a_logSizeMaxBytes) {
            bRv = log.resize(0);
            if (!bRv) {
                qWarning()
                    << "Utils::debugTest: "
                    << log.errorString() << ", "
                    << qTRACE_VAR(logPath);
            }
        }

        QTextStream stream(&log);
        stream.setCodec("UTF-8");
        stream << msg;
    }
#endif

    // write to std::cout
    qWarning() << msg;
}
//-------------------------------------------------------------------------------------------------
void
Utils::debugTracer(
    QtMsgType                 a_type,
    const QMessageLogContext &a_context,
    cQString                 &a_msg
)
{
    QString msg = QString("%1 (%2:%3)")
                .arg(a_msg.toLocal8Bit().constData())
                .arg(a_context.file)
                .arg(a_context.line);

    switch (a_type) {
    case QtDebugMsg:
        msg = QString("Debug:    %1").arg(msg);
        break;
    case QtWarningMsg:
        msg = QString("Warning:  %1").arg(msg);
        break;
    case QtCriticalMsg:
        msg = QString("Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        msg = QString("Fatal:    %1").arg(msg);
        abort();
    default:
        msg = QString("Unknown:  %1").arg(msg);
        break;
    }

    // write to file
    {
    #if 0
        // TODO: Utils::debugTracer() - trace file path
        QFile outFile( Application::debugTracePath() );
        outFile.open(QIODevice::WriteOnly | QIODevice::Append);

        QTextStream ts(&outFile);
        ts << msg << endl;
    #endif
    }

    // write to std::out
    {
        qWarning() << msg;
    }

#if defined(Q_OS_WIN)
    // write to debbuger
    {
        (void)::OutputDebugStringW(msg.toStdWString().c_str());
    }
#endif
}
//-------------------------------------------------------------------------------------------------


/**************************************************************************************************
*   other
*
**************************************************************************************************/

//-------------------------------------------------------------------------------------------------
void
Utils::sleepMsec(
    cint &a_timeoutMsec
)
{
    qTEST(a_timeoutMsec > 0);

#if defined(Q_OS_WIN)
    (void)::Sleep( DWORD(a_timeoutMsec) );
#else
    struct timespec tsTime = {
        a_timeoutMsec / 1000,
        (a_timeoutMsec % 1000) * 1000 * 1000
    };

    ::nanosleep(&tsTime, Q_NULLPTR);
#endif
}
//-------------------------------------------------------------------------------------------------

} // namespace qtlib
