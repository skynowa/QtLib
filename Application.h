/**
 * \file   Application.h
 * \brief  application settings
 */


#pragma once

#include "Common.h"
#include "Utils.h"
#include <QSharedMemory>
#include <QStringList>
//-------------------------------------------------------------------------------------------------
namespace qtlib
{

class Application :
    public QApplication
    /// application
{
    Q_OBJECT
    Q_DISABLE_COPY(Application)

public:
             Application(int &argc, char **argv, cQString &guid);
        ///< constructor
    virtual ~Application();
        ///< destructor

    bool isRunnig() const;
        ///< is application running
    bool isMaster() const;
        ///<
    bool sendMessage(cQString &message) const;
        ///<

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

public Q_SLOTS:
    void slot_checkForMessage();

Q_SIGNALS:
    void messageAvailable(QStringList messages);

private:
    cQString              _guid;
    mutable QSharedMemory _locker;
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
#define qlApp  ( static_cast<qtlib::Application *>(QCoreApplication::instance()) )
//-------------------------------------------------------------------------------------------------
