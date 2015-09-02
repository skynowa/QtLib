/**
 * \file   Application.cpp
 * \brief  application
 */


#include "Application.h"

#include "RunGuard.h"


/**************************************************************************************************
*   private, consts
*
**************************************************************************************************/

//-------------------------------------------------------------------------------------------------
namespace
{

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


namespace qtlib
{

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
    _guid       (a_guid),
    _locker     (a_guid)
{
    qTEST(!a_guid.isEmpty());

    // set codecs
    {
        QTextCodec *codec = QTextCodec::codecForName(::localeCodec);
        qTEST_PTR(codec);

        QTextCodec::setCodecForLocale(codec);
    }

    // when can create it only if it doesn't exist
    if ( _locker.create(5000) ) {
        _locker.lock();
        {
            *(char*)_locker.data() = '\0';
        }
        _locker.unlock();

        // _is_running = false;

        // start checking for messages of other instances
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(checkForMessage()));
        timer->start(200);
    }
    // it exits, so we can attach it
    else if ( _locker.attach() ){
        // _is_running = true;
    }
    else {
        // error
    }
}
//-------------------------------------------------------------------------------------------------
/* virtual */
Application::~Application()
{
}
//-------------------------------------------------------------------------------------------------
bool
Application::isRunnig() const
{
    static RunGuard guard(_guid);

    return !guard.tryToRun();
}
//-------------------------------------------------------------------------------------------------
bool
Application::isMaster() const
{
    return !isRunnig();
}
//-------------------------------------------------------------------------------------------------
bool
Application::sendMessage(
    cQString &a_message
) const
{
    // we cannot send mess if we are master process
    if ( isMaster() ){
        return false;
    }

    QByteArray byteArray;
    byteArray.append(char(a_message.size()));
    byteArray.append(a_message.toUtf8());
    byteArray.append('\0');

    _locker.lock();
    {
        char *to = (char *)_locker.data();
        while (*to != '\0') {
            int sizeToRead = int(*to);
            to += sizeToRead + 1;
        }

        const char *from = byteArray.data();
        ::memcpy(to, from, qMin(_locker.size(), byteArray.size()));
    }
    _locker.unlock();

    return true;
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
void
Application::checkForMessage()
{
    QStringList arguments;

    _locker.lock();
    {
        char *from = (char *)_locker.data();

        while (*from != '\0') {
            int sizeToRead = int(*from);
            ++ from;

            QByteArray byteArray = QByteArray(from, sizeToRead);
            byteArray[sizeToRead] = '\0';
            from += sizeToRead;

            arguments << QString::fromUtf8( byteArray.constData() );
        }

        *(char *)_locker.data() = '\0';
    }
    _locker.unlock();

    if ( !arguments.isEmpty() ) {
        Q_EMIT messageAvailable(arguments);
    }
}
//-------------------------------------------------------------------------------------------------

} // namespace qtlib
