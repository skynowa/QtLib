/**
 * \file   CUtils.cpp
 * \brief
 */


#include "CUtils.h"


#if defined(Q_WS_WIN)
    #include <windows.h>
#else

#endif

#include <QDomDocument>
#include <QTextStream>
#include <Phonon/AudioOutput>
#include <Phonon/MediaObject>


/****************************************************************************
*   private
*
*****************************************************************************/

//---------------------------------------------------------------------------
CUtils::CUtils() {

}
//---------------------------------------------------------------------------
CUtils::~CUtils() {

}
//---------------------------------------------------------------------------


/****************************************************************************
*   public
*
*****************************************************************************/


/****************************************************************************
*   GUI
*
*****************************************************************************/

//---------------------------------------------------------------------------
/* static */
bool
CUtils::setApplicationSingle(
    const QString &a_applicationGuid
)
{
    bool bRv = false;

    static QSharedMemory smLocker(a_applicationGuid);

    bRv = smLocker.attach();
    qCHECK_RET(true == bRv, false);

    bRv = smLocker.create(1);
    qCHECK_RET(false == bRv, false);

    return true;
}
//---------------------------------------------------------------------------
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

    const int x = (desktop->width()  - a_widget->width())  / 2;
    const int y = (desktop->height() - a_widget->height()) / 2;

    a_widget->setGeometry(x, y, a_widget->width(), a_widget->height());
#endif

    QRect rcRect = QStyle::alignedRect(
                        Qt::LeftToRight, Qt::AlignCenter, a_widget->size(),
                        qApp->desktop()->availableGeometry());

    a_widget->setGeometry(rcRect);
}
//---------------------------------------------------------------------------
/* static */
void
CUtils::widgetAlignTopCenter(
    QWidget *a_widget
)
{
    Q_ASSERT(NULL != a_widget);

    QDesktopWidget *desktop = QApplication::desktop();
    Q_ASSERT(NULL != desktop);

    const int x = (desktop->width() - a_widget->width()) / 2;
    const int y = 0;

    a_widget->setGeometry(x, y, a_widget->width(), a_widget->height());
}
//---------------------------------------------------------------------------
/* static */
void
CUtils::applicationActivate(
    const QString &a_className,
    const QString &a_windowName
)
{
#if defined(Q_WS_WIN)
    HWND hWnd = ::FindWindow(qQS2S(a_className).c_str(), qQS2S(a_windowName).c_str());
    if (NULL != hWnd) {
        BOOL blRv = ::SetForegroundWindow(hWnd);
        Q_ASSERT((BOOL)FALSE != blRv);

        ::Beep(400, 400);
    }
#else
    // TODO: activation application window
#endif
}
//---------------------------------------------------------------------------


/****************************************************************************
*   DB
*
*****************************************************************************/

//---------------------------------------------------------------------------
/* static */
int
CUtils::sqlTableModelRowCount(
    QSqlTableModel *a_model
)
{
    qCHECK_RET(NULL == a_model, 0);

    for ( ; a_model->canFetchMore(); ) {
        a_model->fetchMore();
    }

    return a_model->rowCount();
}
//---------------------------------------------------------------------------
/* static */
void
CUtils::importCsv(
    const QString          &a_filePath,
    QSqlTableModel         *a_sqlTableModel,
    const QVector<QString> &a_fieldNames,
    const QString          &a_columnSeparator
)
{
    bool bRv = false;

    // read file
    QStringList slFile;

    {
        QFile fileCSV(a_filePath);

        bRv = fileCSV.open(QFile::ReadOnly);
        Q_ASSERT(true == bRv);

        QString data = fileCSV.readAll();
        slFile = data.split("\n");

        fileCSV.close();

        qCHECK_DO(true == slFile.isEmpty(), return);
    }

    // file -> DB
    for (int i = 0; i < slFile.size(); ++ i) {
        const QStringList cslRow = slFile.at(i).split(a_columnSeparator);

        // iTargetRow
        int iTargetRow = CUtils::sqlTableModelRowCount(a_sqlTableModel) - 1;

        // srRecord
        QSqlRecord srRecord;

        for (int x = 0; x < a_fieldNames.size(); ++ x) {
            srRecord.append(QSqlField(a_fieldNames.at(x)));
            srRecord.setValue(a_fieldNames.at(x), cslRow.at(x));
        }

        bRv = a_sqlTableModel->insertRecord(iTargetRow, srRecord);
        Q_ASSERT(true == bRv);

        bRv = a_sqlTableModel->submitAll();
        Q_ASSERT(true == bRv);
    }
}
//---------------------------------------------------------------------------
/* static */
void
CUtils::exportCsv(
    const QString          &a_filePath,
    QSqlTableModel         *a_sqlTableModel,
    const QVector<QString> &a_fieldNames,
    const QString          &a_columnSeparator
)
{
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
        int iRealRowCount = CUtils::sqlTableModelRowCount(a_sqlTableModel);

        for (int i = 0; i < iRealRowCount; ++ i) {
            for (int x = 0; x < a_fieldNames.size(); ++ x) {
                sCsv.push_back( a_sqlTableModel->record(i).value( a_fieldNames.at(x) ).toString() );
                sCsv.push_back( a_columnSeparator );
            }

            sCsv.push_back( "\n" );
        }
    }

    // write to file
    {
        QFile fileCSV(a_filePath);

        bool bRv = fileCSV.open(QFile::WriteOnly | QIODevice::Text);
        Q_ASSERT(true == bRv);

        QTextStream stream(&fileCSV);

        stream.setCodec("UTF-8");
        stream << sCsv;
    }
}
//---------------------------------------------------------------------------
/* static */
void
CUtils::dbFilter(
    QSqlQueryModel    *sqlQueryModel,
    const QString     &a_tableName,
    const db_fields_t &a_fields,
    const QString     &a_sqlStrJoin,
    const QString     &a_sqlStrWhere,
    const QString     &a_sqlStrOrderBy
)
{
    Q_ASSERT(NULL != sqlQueryModel);
    Q_ASSERT(false == a_tableName.isEmpty());
    Q_ASSERT(false == a_fields.isEmpty());
    // a_sqlStrJoin - n/a
    // a_sqlStrWhere - n/a
    // a_sqlStrOrderBy - n/a

    QString sSqlStr;

    //-------------------------------------
    // is field values empty
    bool bIsAllFieldsEmpty = true;

    for (int i = 0; i < a_fields.size(); ++ i) {
        QString sCtrlValue = a_fields.at(i).second;

        qCHECK_DO(false == sCtrlValue.isEmpty(), bIsAllFieldsEmpty = false; break);
    }

    //-------------------------------------
    // build query
    {
        if (true == bIsAllFieldsEmpty) {
            sSqlStr = QString("SELECT * FROM %1 %2")
                            .arg(a_tableName)
                            .arg(a_sqlStrJoin);
        } else {
            sSqlStr = QString("SELECT * FROM %1 %2 WHERE")
                            .arg(a_tableName)
                            .arg(a_sqlStrJoin);

            bool bIsFirstNotEmptyField = true;

            for (int i = 0; i < a_fields.size(); ++ i) {
                QString sFieldName = a_fields.at(i).first;
                QString sCtrlValue = a_fields.at(i).second;

                qCHECK_DO(true == sCtrlValue.isEmpty(), continue);

                // 1-st field is empty
                if (true == bIsFirstNotEmptyField) {
                    sSqlStr += QString(" (%1 LIKE '%%2%')")
                                    .arg(sFieldName)
                                    .arg(sCtrlValue);

                    bIsFirstNotEmptyField = false;
                    continue;
                }

                sSqlStr += QString(" AND (%1 LIKE '%%2%')")
                                .arg(sFieldName)
                                .arg(sCtrlValue);
            }
        }
    }

    //-------------------------------------
    // a_sqlStrWhere
    if (false == a_sqlStrWhere.isEmpty()) {
        if (true == bIsAllFieldsEmpty) {
            sSqlStr += " WHERE (" + a_sqlStrWhere + ")";
        } else {
            sSqlStr += " AND ("   + a_sqlStrWhere + ")";
        }
    }

    //-------------------------------------
    // a_sqlStrOrderBy
    if (false == a_sqlStrOrderBy.isEmpty()) {
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
//---------------------------------------------------------------------------


/****************************************************************************
*   web
*
*****************************************************************************/

//---------------------------------------------------------------------------
/* static */
QString
CUtils::googleTranslate(
    const QString &a_textFrom,
    const QString &a_langFrom,
    const QString &a_langTo
)
{
    QString sRv;

    // request to Google
    QString sReply;

    {
        const QString csUrl =
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
        delete nrReply; nrReply = NULL;

        Q_ASSERT(!sReply.isEmpty());
    }

    // parse reply
    QStringList lstReply;

    {
        QString sText = sReply;
        sText.replace("<br>", "~");
        sText.replace("~~", "*");
        sText.replace(QObject::tr("Словарь:"), QObject::tr(""));

        QDomDocument document;
        document.setContent(sText);

        QDomNodeList docList = document.elementsByTagName("div");
        for (int i = 0; i < docList.count(); ++ i) {
            lstReply.append(docList.at(i).toElement().text());
        }

        QString sStr = lstReply.at(4);
        if (false == sStr.contains(QObject::tr("Google"))) {
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
//---------------------------------------------------------------------------
/* static */
void
CUtils::googleSpeech(
    const QString &a_text,
    const QString &a_lang,
    const QString &a_filePath
)
{
    // request to Google
    {
        const QString         csUrl = "http://translate.google.ru/translate_tts?&q=" + a_text + "&tl=" + a_lang;
        const QUrl            curUrl(csUrl);
        QNetworkAccessManager nmManager;
        const QNetworkRequest cnrRequest(curUrl);

        QNetworkReply *nrReply = nmManager.get(cnrRequest);
        Q_ASSERT(NULL != nrReply);

        do {
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        }
        while (! nrReply->isFinished());

        // write to audio file
        {
            QFile file(a_filePath);

            bool bRv = file.open(QIODevice::WriteOnly);
            Q_ASSERT(bRv);

            file.write(nrReply->readAll());
        }

        nrReply->close();
        delete nrReply; nrReply = NULL;
    }

    // play audio file
    {
        Phonon::MediaObject *moPlayer = Phonon::createPlayer(Phonon::MusicCategory, Phonon::MediaSource(a_filePath));
        Q_ASSERT(NULL != moPlayer);

        // for signal slot mechanism
        // connect(moPlayer, SIGNAL( finished() ),
        //         moPlayer, SLOT  ( deleteLater() ));

        moPlayer->play();

        // wait for finish
        for (bool bRv = true; bRv; ) {
            Phonon::State stState = moPlayer->state();
            if (Phonon::LoadingState == stState ||
                Phonon::PlayingState == stState)
            {
                bRv = true;
            } else {
                Q_ASSERT(Phonon::PausedState == stState);

                bRv = false;
            }

            sleep(100);

            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        }

        delete moPlayer;    moPlayer = NULL;
    }
}
//---------------------------------------------------------------------------


/****************************************************************************
*   grafics
*
*****************************************************************************/

//---------------------------------------------------------------------------
/* static */
void
CUtils::imageConvert(
    const QString    &a_filePathIn,
    QByteArray       *a_baPhoto,
    const QByteArray &a_format /* = "JPEG" */
)
{
    QImage       image (a_filePathIn);
    QBuffer      buffer(a_baPhoto);
    QImageWriter writer(&buffer, a_format);

    /// writer.setCompression(9);

    bool bRv = writer.write(image);
    Q_ASSERT(true == bRv);

    buffer.close();
}
//---------------------------------------------------------------------------


/****************************************************************************
*   converters
*
*****************************************************************************/

//---------------------------------------------------------------------------
/* static */
std::wstring
CUtils::toStdWString(
    const QString &a_str
)
{
#ifdef _MSC_VER
    return std::wstring(reinterpret_cast<const wchar_t *>( a_str.utf16() ));
#else
    return a_str.toStdWString();
#endif
}
//---------------------------------------------------------------------------
/* static */
QString
CUtils::fromStdWString(
    const std::wstring &a_str
)
{
#ifdef _MSC_VER
    return QString::fromUtf16(reinterpret_cast<const ushort *>( a_str.c_str() ));
#else
    return QString::fromStdWString(a_str);
#endif
}
//---------------------------------------------------------------------------


/****************************************************************************
*   other
*
*****************************************************************************/

//---------------------------------------------------------------------------
void
CUtils::sleep(
    const int &a_timeoutMs
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
//---------------------------------------------------------------------------
