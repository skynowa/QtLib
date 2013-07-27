/**
 * \file   CApplication.cpp
 * \brief  application
 */


#include "CApplication.h"


/*******************************************************************************
*   private, consts
*
*******************************************************************************/

//------------------------------------------------------------------------------
namespace {
    cQString    APP_DIR_PLUGIN_PLATFORMS    = "platforms";
    cQString    APP_DIR_PLUGIN_SQLDRIVERS   = "sqldrivers";
    cQString    APP_DIR_PLUGIN_IMAGEFORMATS = "imageformats";

#if defined(Q_WS_WIN)
    cQByteArray APP_LOCALE_CODEC            = "Windows-1251";
#else
    cQByteArray APP_LOCALE_CODEC            = "UTF-8";
#endif

    cQString    INI_FILE_EXT                = ".ini";
    cQString    LANGS_DIR_NAME              = "Langs";
    cQString    BACKUP_DIR_NAME             = "Backup";
    cQString    DB_DIR_NAME                 = "Db";
    cQString    DB_FILE_EXT                 = ".db";
    cQString    APP_TRACE_PATH              = "trace.log";
}
//------------------------------------------------------------------------------

/*******************************************************************************
*   public
*
*******************************************************************************/

//------------------------------------------------------------------------------
CApplication::CApplication(
    int   &a_argc,
    char **a_argv
) :
    QApplication(a_argc, a_argv)
{
    // set codecs
    {
        QTextCodec *codec = QTextCodec::codecForName(APP_LOCALE_CODEC);
        qTEST(NULL != codec);

        QTextCodec::setCodecForLocale(codec);
    }
}
//------------------------------------------------------------------------------
/* virtual */
CApplication::~CApplication()
{
}
//------------------------------------------------------------------------------

/*******************************************************************************
*   public, static
*
*******************************************************************************/

//------------------------------------------------------------------------------
/* static */
QString
CApplication::iniFilePath()
{
    cQString basename = QFileInfo( applicationFilePath() ).baseName();

    return applicationDirPath() + QDir::separator() + basename + INI_FILE_EXT;
}
//------------------------------------------------------------------------------
/* static */
QString
CApplication::langsDirPath()
{
    return applicationDirPath() + QDir::separator() + LANGS_DIR_NAME;
}
//------------------------------------------------------------------------------
/* static */
QString
CApplication::dbDirPath()
{
    return applicationDirPath() + QDir::separator() + DB_DIR_NAME;
}
//------------------------------------------------------------------------------
/* static */
QString
CApplication::dbFilePath()
{
    return dbDirPath() + QDir::separator() + applicationName() + DB_FILE_EXT;
}
//------------------------------------------------------------------------------
/* static */
QString
CApplication::dbBackupDirPath()
{
    return dbDirPath() + QDir::separator() + BACKUP_DIR_NAME;
}
//------------------------------------------------------------------------------
/* static */
QString
CApplication::debugTracePath()
{
    return applicationDirPath() + QDir::separator() + APP_TRACE_PATH;
}
//------------------------------------------------------------------------------
/* static */
QString
CApplication::pluginPlatformsDirPath()
{
    return applicationDirPath() + QDir::separator() + APP_DIR_PLUGIN_PLATFORMS;
}
//------------------------------------------------------------------------------
/* static */
QString
CApplication::pluginSqlDriversDirPath()
{
    return applicationDirPath() + QDir::separator() + APP_DIR_PLUGIN_SQLDRIVERS;
}
//------------------------------------------------------------------------------
/* static */
QString
CApplication::pluginImageFormatsDirPath()
{
    return applicationDirPath() + QDir::separator() + APP_DIR_PLUGIN_IMAGEFORMATS;
}
//------------------------------------------------------------------------------
/* static */
void
CApplication::windowActivate(
    cQString &a_appWndClass,
    cQString &a_appName
)
{
    CUtils::applicationActivate(a_appWndClass, a_appName);
}
//------------------------------------------------------------------------------
/* static */
bool
CApplication::isRunnig(
    cQString &a_appGuid
)
{
    return CUtils::setApplicationSingle(a_appGuid);
}
//------------------------------------------------------------------------------
/* static */
bool
CApplication::selfCheck()
{
    bool bRv = false;

    bRv = QDir( pluginPlatformsDirPath() ).isReadable();
    if (!bRv) {
        std::wcerr << "Missing plugin directory "
                   << pluginPlatformsDirPath().toStdWString() << std::endl;
        return false;
    }

    bRv = QDir( pluginSqlDriversDirPath() ).isReadable();
    if (!bRv) {
        std::wcerr << "Missing plugin directory "
                   << pluginSqlDriversDirPath().toStdWString() << std::endl;
        return false;
    }

    bRv = QDir( pluginImageFormatsDirPath() ).isReadable();
    if (!bRv) {
        std::wcerr << "Missing plugin directory "
                   << pluginImageFormatsDirPath().toStdWString() << std::endl;
        return false;
    }

    return true;
}
//------------------------------------------------------------------------------
