/**
 * \file   Application.cpp
 * \brief  application
 */


#include "Application.h"


/**************************************************************************************************
*   private, consts
*
**************************************************************************************************/

//-------------------------------------------------------------------------------------------------
namespace {

cQString    dirPluginPlatform     = "platforms";
cQString    dirPluginSqlDrivers   = "sqldrivers";
cQString    dirPluginImageFormats = "imageformats";

#if defined(Q_WS_WIN)
cQByteArray localeCodec           = "Windows-1251";
#else
cQByteArray localeCodec           = "UTF-8";
#endif

}
//-------------------------------------------------------------------------------------------------


namespace qtlib {

/**************************************************************************************************
*   public
*
**************************************************************************************************/

//-------------------------------------------------------------------------------------------------
Application::Application(
    int        &a_argc,
    char     **a_argv,
    cQString  &a_guid
) :
    QApplication(a_argc, a_argv),
    _guid       (a_guid)
{
    qTEST(!a_guid.isEmpty());

    // set codecs
    {
        QTextCodec *codec = QTextCodec::codecForName(::localeCodec);
        qTEST_PTR(codec);

        QTextCodec::setCodecForLocale(codec);
    }
}
//-------------------------------------------------------------------------------------------------
/* virtual */
Application::~Application()
{
}
//-------------------------------------------------------------------------------------------------


/**************************************************************************************************
*   public
*
**************************************************************************************************/

//-------------------------------------------------------------------------------------------------
bool
Application::isRunnig() const
{
    static QSharedMemory locker(_guid);

    bool bRv = locker.attach();
    qCHECK_RET(bRv, true);

    bRv = locker.create(1);
    qCHECK_RET(!bRv, true);

    return false;
}
//-------------------------------------------------------------------------------------------------
/* static */
void
Application::windowActivate(
    cQString &a_windowClassName,
    cQString &a_windowName
)
{
    qTEST(!a_windowClassName.isEmpty());
    qTEST(!a_windowName.isEmpty());

#if defined(Q_OS_WIN)
    HWND hWnd = ::FindWindowW(qQS2S(a_windowClassName).c_str(), qQS2S(a_windowName).c_str());
    if (hWnd != Q_NULLPTR) {
        BOOL blRv = ::SetForegroundWindow(hWnd);
        qTEST((BOOL)FALSE != blRv);

        ::Beep(400, 400);
    }
#else
    Q_UNUSED(a_windowClassName);
    Q_UNUSED(a_windowName);

    // TODO: Application::setWindowActivation() - Unix
#endif

#if 0
    actWin->setWindowState(actWin->windowState() & ~Qt::WindowMinimized);
    actWin->raise();
    actWin->activateWindow();
#endif
}
//-------------------------------------------------------------------------------------------------


/**************************************************************************************************
*   public static
*
**************************************************************************************************/

//-------------------------------------------------------------------------------------------------
/* static */
QString
Application::pluginPlatformsDirPath()
{
    return applicationDirPath() + QDir::separator() + ::dirPluginPlatform;
}
//-------------------------------------------------------------------------------------------------
/* static */
QString
Application::pluginSqlDriversDirPath()
{
    return applicationDirPath() + QDir::separator() + ::dirPluginSqlDrivers;
}
//-------------------------------------------------------------------------------------------------
/* static */
QString
Application::pluginImageFormatsDirPath()
{
    return applicationDirPath() + QDir::separator() + ::dirPluginImageFormats;
}
//-------------------------------------------------------------------------------------------------
/* static */
bool
Application::selfCheck()
{
    bool bRv = false;

    bRv = QString(QT_VERSION_STR) == QString( qVersion() );
    if (!bRv) {
        std::wcerr
            << "QtLib/Application: "
            << "Qt compile version \"" << QT_VERSION_STR << "\" and "
            << "Qt runtime version \"" << qVersion()     << "\" mismatch" << std::endl;
        return false;
    }

#if 0
    // TODO: Application::selfCheck()

    bRv = QDir( pluginPlatformsDirPath() ).isReadable();
    if (!bRv) {
        std::wcerr
            << "QtLib/Application: "
            << "Missing plugin directory \""
            << pluginPlatformsDirPath().toStdWString() << "\"" << std::endl;
        return false;
    }

    bRv = QDir( pluginSqlDriversDirPath() ).isReadable();
    if (!bRv) {
        std::wcerr
            << "QtLib/Application: "
            << "Missing plugin directory \""
            << pluginSqlDriversDirPath().toStdWString() << "\"" << std::endl;
        return false;
    }

    bRv = QDir( pluginImageFormatsDirPath() ).isReadable();
    if (!bRv) {
        std::wcerr
            << "QtLib/Application: "
            << "Missing plugin directory \""
            << pluginImageFormatsDirPath().toStdWString() << "\"" << std::endl;
        return false;
    }
#endif

    return true;
}
//-------------------------------------------------------------------------------------------------

} // namespace qtlib
