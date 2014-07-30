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
             Application(int &argc, char **argv, cQString &guid);
        ///< constructor
    virtual ~Application();
        ///< destructor

    bool     isRunnig() const;
        ///< is application running
    void     windowActivate(cQString &windowClassName, cQString &windowName);
        ///< activate main window

    // static
    static
    QString  pluginPlatformsDirPath();
        ///< plugin platforms directory path
    static
    QString  pluginSqlDriversDirPath();
        ///< plugin sqldrivers  directory path
    static
    QString  pluginImageFormatsDirPath();
        ///< plugin imageformats  directory path

    static
    bool     selfCheck();
        ///< self check

private:
    cQString _guid;
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
#define qlApp  ( static_cast<qtlib::Application *>(QCoreApplication::instance()) )
//-------------------------------------------------------------------------------------------------
#endif // QtLib_CApplicationH
