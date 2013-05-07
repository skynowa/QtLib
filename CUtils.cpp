/**
 * \file   CUtils.cpp
 * \brief
 */


#include "CUtils.h"

#include "CApplication.h"

#if defined(Q_OS_WIN)
    #include <windows.h>
#endif

#include <QDomDocument>
#include <QTextStream>


/*******************************************************************************
*   private
*
*******************************************************************************/

//------------------------------------------------------------------------------
CUtils::CUtils()
{

}
//------------------------------------------------------------------------------
CUtils::~CUtils()
{

}
//------------------------------------------------------------------------------


/*******************************************************************************
*   public
*
*******************************************************************************/


/*******************************************************************************
*   GUI
*
*******************************************************************************/

//------------------------------------------------------------------------------
/* static */
bool
CUtils::setApplicationSingle(
    cQString &a_applicationGuid
)
{
    qTEST(!a_applicationGuid.isEmpty());

    bool bRv = false;

    static QSharedMemory smLocker(a_applicationGuid);

    bRv = smLocker.attach();
    qCHECK_RET(bRv, false);

    bRv = smLocker.create(1);
    qCHECK_RET(!bRv, false);

    return true;
}
//------------------------------------------------------------------------------
/* static */
void
CUtils::widgetAlignCenter(
    QWidget *a_widget
)
{
    qTEST(NULL != a_widget);

#if 0
    QDesktopWidget *desktop = QApplication::desktop();
    qTEST(NULL != desktop);

    cint x = (desktop->width()  - a_widget->width())  / 2;
    cint y = (desktop->height() - a_widget->height()) / 2;

    a_widget->setGeometry(x, y, a_widget->width(), a_widget->height());
#endif

    QRect rcRect = QStyle::alignedRect(
                        Qt::LeftToRight, Qt::AlignCenter, a_widget->size(),
                        qApp->desktop()->availableGeometry());

    a_widget->setGeometry(rcRect);
}
//------------------------------------------------------------------------------
/* static */
void
CUtils::widgetAlignTopCenter(
    QWidget *a_widget
)
{
    qTEST(NULL != a_widget);

    QDesktopWidget *desktop = QApplication::desktop();
    qTEST(NULL != desktop);

    cint x = (desktop->width() - a_widget->width()) / 2;
    cint y = 0;

    a_widget->setGeometry(x, y, a_widget->width(), a_widget->height());
}
//------------------------------------------------------------------------------
/* static */
void
CUtils::applicationActivate(
    cQString &a_className,
    cQString &a_windowName
)
{
    qTEST(!a_className.isEmpty());
    qTEST(!a_windowName.isEmpty());

#if defined(Q_OS_WIN)
    HWND hWnd = ::FindWindowW(
                    qQS2S(a_className).c_str(),
                    qQS2S(a_windowName).c_str());
    if (NULL != hWnd) {
        BOOL blRv = ::SetForegroundWindow(hWnd);
        qTEST((BOOL)FALSE != blRv);

        ::Beep(400, 400);
    }
#else
    Q_UNUSED(a_className);
    Q_UNUSED(a_windowName);

    // TODO: activation application window
#endif
}
//------------------------------------------------------------------------------
void
CUtils::widgetAlwaysOnTop(
    QWidget *a_widget,
    cbool   &a_isChecked
)
{
    qTEST(NULL != a_widget);
    // a_isChecked - n/a

    Qt::WindowFlags flags = a_widget->windowFlags();
    if (a_isChecked) {
        flags |= (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    } else {
        flags &= ~(Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    }

    a_widget->setWindowFlags(flags);
}
//------------------------------------------------------------------------------


/*******************************************************************************
*   DB
*
*******************************************************************************/

//------------------------------------------------------------------------------
/* static */
int
CUtils::sqlTableModelRowCount(
    QSqlTableModel *a_model,
    cbool          &a_isFetchAllRows /* = false */
)
{
    // a_model - n/a
    qCHECK_RET(NULL == a_model, 0);

    if (a_isFetchAllRows) {
        for ( ; a_model->canFetchMore(); ) {
            a_model->fetchMore();
        }
    }

    return a_model->rowCount();
}
//------------------------------------------------------------------------------
/* static */
void
CUtils::importCsv(
    cQString               &a_filePath,
    QSqlTableModel         *a_sqlTableModel,
    const QVector<QString> &a_fieldNames,
    cQString               &a_columnSeparator
)
{
    qTEST(!a_filePath.isEmpty());
    qTEST(NULL != a_sqlTableModel);
    qTEST(!a_fieldNames.isEmpty());
    qTEST(!a_columnSeparator.isEmpty());

    bool bRv = false;

    // read file
    QStringList slFile;

    {
        QFile fileCSV(a_filePath);

        bRv = fileCSV.open(QFile::ReadOnly);
        qTEST(bRv);

        QString data = fileCSV.readAll();
        slFile = data.split("\n");

        fileCSV.close();

        qCHECK_DO(slFile.isEmpty(), return);
    }

    // file -> DB
    for (int i = 0; i < slFile.size(); ++ i) {
        cQStringList cslRow = slFile.at(i).split(a_columnSeparator);

        // iTargetRow
        cint ciTargetRow = CUtils::sqlTableModelRowCount(a_sqlTableModel) - 1;

        // srRecord
        QSqlRecord srRecord;

        for (int x = 0; x < a_fieldNames.size(); ++ x) {
            srRecord.append(QSqlField(a_fieldNames.at(x)));
            srRecord.setValue(a_fieldNames.at(x), cslRow.at(x));
        }

        bRv = a_sqlTableModel->insertRecord(ciTargetRow, srRecord);
        qTEST(bRv);

        bRv = a_sqlTableModel->submitAll();
        qTEST(bRv);
    }
}
//------------------------------------------------------------------------------
/* static */
void
CUtils::exportCsv(
    cQString               &a_filePath,
    QSqlTableModel         *a_sqlTableModel,
    const QVector<QString> &a_fieldNames,
    cQString               &a_columnSeparator
)
{
    qTEST(!a_filePath.isEmpty());
    qTEST(NULL != a_sqlTableModel);
    qTEST(!a_fieldNames.isEmpty());
    qTEST(!a_columnSeparator.isEmpty());

    // DB -> text
    QString sCsv;

    // DB fields -> CSV header
    for (int x = 0; x < a_fieldNames.size(); ++ x) {
        sCsv.push_back( a_fieldNames.at(x) );
        sCsv.push_back( a_columnSeparator );
    }
    sCsv.push_back( "\n" );

    // DB -> file
    {
        cint ciRealRowCount = CUtils::sqlTableModelRowCount(a_sqlTableModel);

        for (int i = 0; i < ciRealRowCount; ++ i) {
            for (int x = 0; x < a_fieldNames.size(); ++ x) {
                sCsv.push_back( a_sqlTableModel->record(i)
                                    .value( a_fieldNames.at(x) ).toString() );
                sCsv.push_back( a_columnSeparator );
            }

            sCsv.push_back( "\n" );
        }
    }

    // write to file
    {
        QFile fileCSV(a_filePath);

        bool bRv = fileCSV.open(QFile::WriteOnly | QIODevice::Text);
        qTEST(bRv);

        QTextStream stream(&fileCSV);

        stream.setCodec("UTF-8");
        stream << sCsv;
    }
}
//------------------------------------------------------------------------------
/* static */
void
CUtils::dbFilter(
    QSqlQueryModel *sqlQueryModel,
    cQString       &a_tableName,
    cdb_fields_t   &a_fields,
    cQString       &a_sqlStrJoin,
    cQString       &a_sqlStrWhere,
    cQString       &a_sqlStrOrderBy
)
{
    qTEST(NULL != sqlQueryModel);
    qTEST(!a_tableName.isEmpty());
    qTEST(!a_fields.isEmpty());
    // a_sqlStrJoin - n/a
    // a_sqlStrWhere - n/a
    // a_sqlStrOrderBy - n/a

    QString sSqlStr;

    //-------------------------------------
    // is field values empty
    bool bIsAllFieldsEmpty = true;

    for (int i = 0; i < a_fields.size(); ++ i) {
        cQString csCtrlValue = a_fields.at(i).second;

        qCHECK_DO(!csCtrlValue.isEmpty(), bIsAllFieldsEmpty = false; break);
    }

    //-------------------------------------
    // build query
    {
        if (bIsAllFieldsEmpty) {
            sSqlStr = QString("SELECT * FROM %1 %2")
                            .arg(a_tableName)
                            .arg(a_sqlStrJoin);
        } else {
            sSqlStr = QString("SELECT * FROM %1 %2 WHERE")
                            .arg(a_tableName)
                            .arg(a_sqlStrJoin);

            bool bIsFirstNotEmptyField = true;

            for (int i = 0; i < a_fields.size(); ++ i) {
                cQString csFieldName = a_fields.at(i).first;
                cQString csCtrlValue = a_fields.at(i).second;

                qCHECK_DO(csCtrlValue.isEmpty(), continue);

                // 1-st field is empty
                if (bIsFirstNotEmptyField) {
                    sSqlStr += QString(" (%1 LIKE '%%2%')")
                                    .arg(csFieldName)
                                    .arg(csCtrlValue);

                    bIsFirstNotEmptyField = false;
                    continue;
                }

                sSqlStr += QString(" AND (%1 LIKE '%%2%')")
                                .arg(csFieldName)
                                .arg(csCtrlValue);
            }
        }
    }

    //-------------------------------------
    // a_sqlStrWhere
    if (!a_sqlStrWhere.isEmpty()) {
        if (bIsAllFieldsEmpty) {
            sSqlStr += " WHERE (" + a_sqlStrWhere + ")";
        } else {
            sSqlStr += " AND ("   + a_sqlStrWhere + ")";
        }
    }

    //-------------------------------------
    // a_sqlStrOrderBy
    if (!a_sqlStrOrderBy.isEmpty()) {
        sSqlStr += " " + a_sqlStrOrderBy + ";";
    }

    //-------------------------------------
    // execute query
    {
        QSqlQueryModel *qmModel = dynamic_cast<QSqlQueryModel *>( sqlQueryModel );
        qTEST(NULL != qmModel);

        qmModel->setQuery(sSqlStr);

        qDebug() << sSqlStr;
    }
}
//------------------------------------------------------------------------------
void
CUtils::dbFieldNames(
    const QSqlDatabase &a_db,           ///< database
    cQString           &a_tableName,    ///< table name
    QStringList        *a_dbFileldNames ///< field names [out]
)
{
    qTEST(a_db.isValid());
    qTEST(!a_tableName.isEmpty());
    qTEST(NULL != a_dbFileldNames);

    QStringList slRv;
    QSqlQuery   qryTableInfo(a_db);

    cQString  csSql = \
        "pragma table_info(" + a_tableName + ");";

    bool bRv = qryTableInfo.exec(csSql);
    qCHECK_REF(bRv, qryTableInfo);

    while (qryTableInfo.next()) {
        slRv << qryTableInfo.value(1).toString();
    }

    slRv.swap(*a_dbFileldNames);
}
//------------------------------------------------------------------------------


/*******************************************************************************
*   web
*
*******************************************************************************/

//------------------------------------------------------------------------------
/* static */
QString
CUtils::googleTranslate(
    cQString &a_textFrom,
    cQString &a_langFrom,
    cQString &a_langTo
)
{
    qTEST(!a_textFrom.isEmpty());
    qTEST(!a_langFrom.isEmpty());
    qTEST(!a_langTo.isEmpty());

    QString sRv;

    // request to Google
    QString sReply;

    {
        cQString csUrl =
            "http://translate.google.com/m?translate_a/t?client=t&text="
            + a_textFrom +
            "&sl="
            + a_langFrom +
            "&tl="
            + a_langTo;

        QNetworkAccessManager nmManager;
        QNetworkRequest       nrRequest(csUrl);

        QNetworkReply *nrReply = nmManager.get(nrRequest);
        qTEST(NULL != nrReply);

        do {
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        }
        while (! nrReply->isFinished());

        sReply = QString::fromUtf8(nrReply->readAll());

        nrReply->close();
        qPTR_DELETE(nrReply);

        qTEST(!sReply.isEmpty());
    }

    // parse reply
    QStringList lstReply;

    {
        QString sText = sReply;
        sText.replace("<br>", "~");
        sText.replace("~~", "*");
        sText.replace("Словарь:", QObject::tr(""));

        QDomDocument document;
        document.setContent(sText);

        QDomNodeList docList = document.elementsByTagName("div");
        for (int i = 0; i < docList.count(); ++ i) {
            lstReply.append(docList.at(i).toElement().text());
        }

        QString sStr = lstReply.at(4);
        if (!sStr.contains(QObject::tr("Google"))) {
            sStr.replace("~", "\n    - ");
            sStr.replace("*", "\n\n");
            sStr.remove(sStr.count() - 2, 2);
        } else {
            sStr.clear();
        }

        lstReply.append(sStr);

        //
        sRv = lstReply.at(2);
        sRv += "\n\n" + lstReply.last();
    }

    return sRv;
}
//------------------------------------------------------------------------------


/*******************************************************************************
*   grafics
*
*******************************************************************************/

//------------------------------------------------------------------------------
/* static */
void
CUtils::imageConvert(
    cQString    &a_filePathIn,
    QByteArray  *a_photo,
    cQByteArray &a_format /* = "JPEG" */
)
{
    qTEST(!a_filePathIn.isEmpty());
    qTEST(NULL != a_photo);
    qTEST(!a_format.isEmpty());

    QBuffer      buffer(a_photo);
    QImageWriter writer(&buffer, a_format);
    QImage       image (a_filePathIn);

    /// writer.setCompression(9);

    bool bRv = writer.write(image);
    qTEST(bRv);

    buffer.close();
}
//------------------------------------------------------------------------------


/*******************************************************************************
*   converters
*
*******************************************************************************/

//------------------------------------------------------------------------------
/* static */
std::wstring
CUtils::toStdWString(
    cQString &a_str
)
{
    // a_str - n/a

#ifdef _MSC_VER
    return std::wstring(reinterpret_cast<const wchar_t *>( a_str.utf16() ));
#else
    return a_str.toStdWString();
#endif
}
//------------------------------------------------------------------------------
/* static */
QString
CUtils::fromStdWString(
    std_cwstring &a_str
)
{
    // a_str - n/a

#ifdef _MSC_VER
    return QString::fromUtf16(reinterpret_cast<const ushort *>( a_str.c_str() ));
#else
    return QString::fromStdWString(a_str);
#endif
}
//------------------------------------------------------------------------------
/* static */
QString
CUtils::formatBytes(
    culonglong &a_bytes
)
{
    // a_bytes - n/a

    QString sRv;

    culonglong cullTB   = 1024ULL * 1024ULL * 1024ULL * 1024ULL;
    culonglong cullGB   = 1024ULL * 1024ULL * 1024ULL;
    culonglong cullMB   = 1024ULL * 1024ULL;
    culonglong cullKB   = 1024ULL;
    culonglong cullByte = 1ULL;

    if (     a_bytes / cullTB   > 0ULL) {
        sRv.sprintf("%.2f TB",
                    static_cast<double>(a_bytes) / static_cast<double>(cullTB));
    }
    else if (a_bytes / cullGB   > 0ULL) {
        sRv.sprintf("%.2f GB",
                    static_cast<double>(a_bytes) / static_cast<double>(cullGB));
    }
    else if (a_bytes / cullMB   > 0ULL) {
        sRv.sprintf("%.2f MB",
                    static_cast<double>(a_bytes) / static_cast<double>(cullMB));
    }
    else if (a_bytes / cullKB   > 0ULL) {
        sRv.sprintf("%.2f KB",
                    static_cast<double>(a_bytes) / static_cast<double>(cullKB));
    }
    else if (a_bytes / cullByte > 0ULL) {
        sRv.sprintf("%.2f Byte(s)",
                    static_cast<double>(a_bytes) / static_cast<double>(cullByte));
    }
    else {
        sRv.sprintf("%.2f Bit(s)",
                    static_cast<double>(a_bytes));
    }

    return sRv;
}
//------------------------------------------------------------------------------


/*******************************************************************************
*   debug
*
*******************************************************************************/

//------------------------------------------------------------------------------
void
CUtils::debugTest(
    cQString &a_expression,
    cQString &a_filePath,
    culong   &a_fileLine,
    cQString &a_functionName,
    cqint64  &a_logSizeMaxBytes
)
{
    cQString csMsg = QString(
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

    // write to file
    {
        cQString logPath = QApplication::applicationDirPath() + QDir::separator() +
                           QApplication::applicationName() + ".log";
        QFile    log(logPath);

        // truncate log file
        if (log.size() > a_logSizeMaxBytes) {
            bool bRv = log.resize(0);
            qTEST(bRv);
        }

        bool bRv = log.open(QFile::Append | QIODevice::Text);
        qTEST(bRv);

        QTextStream stream(&log);

        stream.setCodec("UTF-8");
        stream << csMsg;
    }
}
//------------------------------------------------------------------------------
void
CUtils::debugTracer(
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
        QFile outFile( CApplication::debugTracePath() );
        outFile.open(QIODevice::WriteOnly | QIODevice::Append);

        QTextStream ts(&outFile);
        ts << msg << endl;
    }

    // write to std::out
    {
        std::wcout << msg.toStdWString() << std::endl;
    }

#if defined(Q_OS_WIN)
    // write to debbuger
    {
        (void)::OutputDebugStringW(msg.toStdWString().c_str());
    }
#endif
}
//------------------------------------------------------------------------------


/*******************************************************************************
*   other
*
*******************************************************************************/

//------------------------------------------------------------------------------
void
CUtils::sleep(
    cint &a_timeoutMs
)
{
    qTEST(a_timeoutMs > 0);

#ifdef Q_OS_WIN
    (void)::Sleep( DWORD(a_timeoutMs) );
#else
    struct timespec tsTime = {
        a_timeoutMs / 1000,
        (a_timeoutMs % 1000) * 1000 * 1000
    };

    ::nanosleep(&tsTime, NULL);
#endif
}
//------------------------------------------------------------------------------
