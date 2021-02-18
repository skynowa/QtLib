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
*   UI
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
    qTEST_NA(a_isChecked);

    Qt::WindowFlags flags = a_widget->windowFlags();
    if (a_isChecked) {
        flags |= (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    } else {
        flags &= ~(Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    }

    a_widget->setWindowFlags(flags);
}
//-------------------------------------------------------------------------------------------------
/* static */
void
Utils::widgetActivate(
    QWidget *a_widget
)
{
    qTEST_PTR(a_widget);

    a_widget->show();
    a_widget->activateWindow();
    a_widget->raise();
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
    qTEST_NA(a_str);

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
    qTEST_NA(a_str);

#if defined(_MSC_VER)
    return QString::fromUtf16(reinterpret_cast<const ushort *>( a_str.c_str() ));
#else
    return QString::fromStdWString(a_str);
#endif
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

#if defined(Q_OS_ANDROID)
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

    ::nanosleep(&tsTime, nullptr);
#endif
}
//-------------------------------------------------------------------------------------------------

} // namespace qtlib
