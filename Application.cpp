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
Application::windowActivate(
    cQString &a_appWndClass,
    cQString &a_appName
)
{
    qtlib::Utils::applicationActivate(a_appWndClass, a_appName);
}
//-------------------------------------------------------------------------------------------------
/* static */
bool
Application::isRunnig(
    cQString &a_appGuid
)
{
    return !qtlib::Utils::setApplicationSingle(a_appGuid);
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
