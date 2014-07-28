/**
 * \file   Application.h
 * \brief  application settings
 */


#ifndef QtLib_CApplicationH
#define QtLib_CApplicationH
//-------------------------------------------------------------------------------------------------
#include "../QtLib/Common.h"
#include "../QtLib/Utils.h"
//-------------------------------------------------------------------------------------------------
namespace qtlib {

class Application :
    public QApplication
    /// application
{
public:
    //      Application();
        ///< constructor
            Application(int &argc, char **argv);
        ///< constructor
    virtual ~Application();
        ///< destructor

    // static
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
    void    setSingle(cQString &guid, bool *isRunnig);
        ///< set application single inststance
    static
    void    windowActivate(cQString &windowClassName, cQString &windowName);
        ///< activate main window
    static
    bool    selfCheck();
        ///< self check
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
#define qlApp  ( static_cast<qtlib::Application *>(QCoreApplication::instance()) )
//-------------------------------------------------------------------------------------------------
#endif // QtLib_CApplicationH
