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
    int   &a_argc,
    char **a_argv
) :
    QApplication(a_argc, a_argv)
{
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
*   public, static
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
void
Application::setSingle(
    cQString &a_guid,
    bool     *a_isRunnig
)
{
    qTEST(!a_guid.isEmpty());
    qTEST(a_isRunnig != Q_NULLPTR);

    bool bRv = false;

    static QSharedMemory locker(a_guid);

    bRv = locker.attach();
    qCHECK_DO(bRv, *a_isRunnig = true; return);

    bRv = locker.create(1);
    qCHECK_DO(!bRv, *a_isRunnig = true; return);

    *a_isRunnig = false;
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

    // TODO: activation application window
#endif
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
