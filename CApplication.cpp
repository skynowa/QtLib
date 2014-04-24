/**
 * \file   CApplication.cpp
 * \brief  application
 */


#include "CApplication.h"


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
CApplication::CApplication(
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
CApplication::~CApplication()
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
CApplication::configFilePath()
{
    cQString basename = QFileInfo( applicationFilePath() ).baseName();

    return applicationDirPath() + QDir::separator() + basename + ::configFileExt;
}
//-------------------------------------------------------------------------------------------------
/* static */
QString
CApplication::langsDirPath()
{
    return applicationDirPath() + QDir::separator() + ::langsDirName;
}
//-------------------------------------------------------------------------------------------------
/* static */
QString
CApplication::dbDirPath()
{
    return applicationDirPath() + QDir::separator() + ::dbDirName;
}
//-------------------------------------------------------------------------------------------------
/* static */
QString
CApplication::dbFilePath()
{
    return dbDirPath() + QDir::separator() + applicationName() + ::dbFileExt;
}
//-------------------------------------------------------------------------------------------------
/* static */
QString
CApplication::dbBackupDirPath()
{
    return dbDirPath() + QDir::separator() + ::backupDirName;
}
//-------------------------------------------------------------------------------------------------
/* static */
QString
CApplication::debugTracePath()
{
    return applicationDirPath() + QDir::separator() + ::tracePath;
}
//-------------------------------------------------------------------------------------------------
/* static */
QString
CApplication::pluginPlatformsDirPath()
{
    return applicationDirPath() + QDir::separator() + ::dirPluginPlatform;
}
//-------------------------------------------------------------------------------------------------
/* static */
QString
CApplication::pluginSqlDriversDirPath()
{
    return applicationDirPath() + QDir::separator() + ::dirPluginSqlDrivers;
}
//-------------------------------------------------------------------------------------------------
/* static */
QString
CApplication::pluginImageFormatsDirPath()
{
    return applicationDirPath() + QDir::separator() + ::dirPluginImageFormats;
}
//-------------------------------------------------------------------------------------------------
/* static */
void
CApplication::windowActivate(
    cQString &a_appWndClass,
    cQString &a_appName
)
{
    CUtils::applicationActivate(a_appWndClass, a_appName);
}
//-------------------------------------------------------------------------------------------------
/* static */
bool
CApplication::isRunnig(
    cQString &a_appGuid
)
{
    return !CUtils::setApplicationSingle(a_appGuid);
}
//-------------------------------------------------------------------------------------------------
/* static */
bool
CApplication::selfCheck()
{
    bool bRv = false;

    bRv = QString(QT_VERSION_STR) == QString( qVersion() );
    if (!bRv) {
        std::wcerr
            << "QtLib/CApplication: "
            << "Qt compile version \"" << QT_VERSION_STR << "\" and "
            << "Qt runtime version \"" << qVersion()     << "\" mismatch" << std::endl;
        return false;
    }

#if 0
    // TODO: CApplication::selfCheck()

    bRv = QDir( pluginPlatformsDirPath() ).isReadable();
    if (!bRv) {
        std::wcerr
            << "QtLib/CApplication: "
            << "Missing plugin directory \""
            << pluginPlatformsDirPath().toStdWString() << "\"" << std::endl;
        return false;
    }

    bRv = QDir( pluginSqlDriversDirPath() ).isReadable();
    if (!bRv) {
        std::wcerr
            << "QtLib/CApplication: "
            << "Missing plugin directory \""
            << pluginSqlDriversDirPath().toStdWString() << "\"" << std::endl;
        return false;
    }

    bRv = QDir( pluginImageFormatsDirPath() ).isReadable();
    if (!bRv) {
        std::wcerr
            << "QtLib/CApplication: "
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
CApplication::versionInfo()
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
CApplication::buildInfo()
{
    return QString("Qt %1")
                .arg(QT_VERSION_STR);
}
//-------------------------------------------------------------------------------------------------
