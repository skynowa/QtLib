/**
 * \file   CUtils.cpp
 * \brief
 */


#include "CUtils.h"


#if defined(Q_OS_WIN)
    #include <windows.h>
#else

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
    Q_ASSERT(!a_applicationGuid.isEmpty());

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
    Q_ASSERT(NULL != a_widget);

#if 0
    QDesktopWidget *desktop = QApplication::desktop();
    Q_ASSERT(NULL != desktop);

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
    Q_ASSERT(NULL != a_widget);

    QDesktopWidget *desktop = QApplication::desktop();
    Q_ASSERT(NULL != desktop);

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
    Q_ASSERT(!a_className.isEmpty());
    Q_ASSERT(!a_windowName.isEmpty());

#if defined(Q_OS_WIN)
    HWND hWnd = ::FindWindowW(
                    qQS2S(a_className).c_str(),
                    qQS2S(a_windowName).c_str());
    if (NULL != hWnd) {
        BOOL blRv = ::SetForegroundWindow(hWnd);
        Q_ASSERT((BOOL)FALSE != blRv);

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
    Q_ASSERT(NULL != a_widget);
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
    QSqlTableModel *a_model
)
{
    // a_model - n/a
    qCHECK_RET(NULL == a_model, 0);

    for ( ; a_model->canFetchMore(); ) {
        a_model->fetchMore();
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
    Q_ASSERT(!a_filePath.isEmpty());
    Q_ASSERT(NULL != a_sqlTableModel);
    Q_ASSERT(!a_fieldNames.isEmpty());
    Q_ASSERT(!a_columnSeparator.isEmpty());

    bool bRv = false;

    // read file
    QStringList slFile;

    {
        QFile fileCSV(a_filePath);

        bRv = fileCSV.open(QFile::ReadOnly);
        Q_ASSERT(bRv);

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
        Q_ASSERT(bRv);

        bRv = a_sqlTableModel->submitAll();
        Q_ASSERT(bRv);
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
    Q_ASSERT(!a_filePath.isEmpty());
    Q_ASSERT(NULL != a_sqlTableModel);
    Q_ASSERT(!a_fieldNames.isEmpty());
    Q_ASSERT(!a_columnSeparator.isEmpty());

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
        Q_ASSERT(bRv);

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
    Q_ASSERT(NULL != sqlQueryModel);
    Q_ASSERT(!a_tableName.isEmpty());
    Q_ASSERT(!a_fields.isEmpty());
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
        Q_ASSERT(NULL != qmModel);

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
    Q_ASSERT(a_db.isValid());
    Q_ASSERT(!a_tableName.isEmpty());
    Q_ASSERT(NULL != a_dbFileldNames);

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
    Q_ASSERT(!a_textFrom.isEmpty());
    Q_ASSERT(!a_langFrom.isEmpty());
    Q_ASSERT(!a_langTo.isEmpty());

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
        Q_ASSERT(NULL != nrReply);

        do {
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        }
        while (! nrReply->isFinished());

        sReply = QString::fromUtf8(nrReply->readAll());

        nrReply->close();
        qPTR_DELETE(nrReply);

        Q_ASSERT(!sReply.isEmpty());
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
    Q_ASSERT(!a_filePathIn.isEmpty());
    Q_ASSERT(NULL != a_photo);
    Q_ASSERT(!a_format.isEmpty());

    QBuffer      buffer(a_photo);
    QImageWriter writer(&buffer, a_format);
    QImage       image (a_filePathIn);

    /// writer.setCompression(9);

    bool bRv = writer.write(image);
    Q_ASSERT(bRv);

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
*   other
*
*******************************************************************************/

//------------------------------------------------------------------------------
void
CUtils::sleep(
    cint &a_timeoutMs
)
{
    Q_ASSERT(a_timeoutMs > 0);

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
