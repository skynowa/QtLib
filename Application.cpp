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

    cQString    configFileExt         = ".cfg";
    cQString    langsDirName          = "Langs";
    cQString    backupDirName         = "Backup";
    cQString    dbDirName             = "Db";
    cQString    dbFileExt             = ".db";
    cQString    tracePath             = "trace.log";
}
//-------------------------------------------------------------------------------------------------


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
Application::configFilePath()
{
    cQString basename = QFileInfo( applicationFilePath() ).baseName();

    return applicationDirPath() + QDir::separator() + basename + ::configFileExt;
}
//-------------------------------------------------------------------------------------------------
/* static */
QString
Application::langsDirPath()
{
    return applicationDirPath() + QDir::separator() + ::langsDirName;
}
//-------------------------------------------------------------------------------------------------
/* static */
QString
Application::dbDirPath()
{
    return applicationDirPath() + QDir::separator() + ::dbDirName;
}
//-------------------------------------------------------------------------------------------------
/* static */
QString
Application::dbFilePath()
{
    return dbDirPath() + QDir::separator() + applicationName() + ::dbFileExt;
}
//-------------------------------------------------------------------------------------------------
/* static */
QString
Application::dbBackupDirPath()
{
    return dbDirPath() + QDir::separator() + ::backupDirName;
}
//-------------------------------------------------------------------------------------------------
/* static */
QString
Application::debugTracePath()
{
    return applicationDirPath() + QDir::separator() + ::tracePath;
}
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
    ::Utils::applicationActivate(a_appWndClass, a_appName);
}
//-------------------------------------------------------------------------------------------------
/* static */
bool
Application::isRunnig(
    cQString &a_appGuid
)
{
    return !::Utils::setApplicationSingle(a_appGuid);
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
/* static */
QString
Application::versionInfo()
{
    return QString("%1 %2 (%3 %4)")
                .arg( applicationName() )
                .arg( applicationVersion() )
                .arg(__TIME__)
                .arg(__DATE__);
}
//-------------------------------------------------------------------------------------------------
/* static */
QString
Application::buildInfo()
{
    return QString("Qt %1")
                .arg(QT_VERSION_STR);
}
//-------------------------------------------------------------------------------------------------
