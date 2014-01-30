/**
 * \file   CApplication.h
 * \brief  application settings
 */


#ifndef QtLib_CApplicationH
#define QtLib_CApplicationH
//-------------------------------------------------------------------------------------------------
#include "../QtLib/Common.h"
#include "../QtLib/CUtils.h"
//-------------------------------------------------------------------------------------------------
class CApplication :
    public QApplication
    ///< application
{
public:
            CApplication(int &argc, char **argv);
        ///< constructor
    virtual ~CApplication();
        ///< destructor

    // static
    static
    QString iniFilePath();
        ///< ini file path
    static
    QString langsDirPath();
        ///< languages directory path
    static
    QString dbDirPath();
        ///< database directory path
    static
    QString dbFilePath();
        ///< database file path
    static
    QString dbBackupDirPath();
        ///< database backup directory path
    static
    QString debugTracePath();
        ///< debug trace path

    static
    QString pluginPlatformsDirPath();
        ///< plugin platforms directory path
    static
    QString pluginSqlDriversDirPath();
        ///< plugin sqldrivers  directory path
    static
    QString pluginImageFormatsDirPath();
        ///< plugin imageformats  directory path

    static
    void    windowActivate(cQString &appWndClass, cQString &appName);
        ///< activate main window
    static
    bool    isRunnig(cQString &appGuid);
        ///< check if application already running
    static
    bool    selfCheck();
        ///< self check
    static
    QString versionInfo();
        ///< full version information
    static
    QString buildInfo();
        ///< build information
};
//-------------------------------------------------------------------------------------------------
#define qCApp  ( static_cast<CApplication *>(QCoreApplication::instance()) )
//-------------------------------------------------------------------------------------------------
#endif // QtLib_CApplicationH
