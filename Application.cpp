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

class RunGuard
    ///< run guard
{
public:
    explicit RunGuard(cQString &key);
            ~RunGuard();

    bool     isAnotherRunning();
    bool     tryToRun();
    void     release();

private:
    cQString _key;
    cQString _memLockKey;
    cQString _sharedmemKey;

    QSharedMemory    _sharedMem;
    QSystemSemaphore _memLock;

    QString  _generateKeyHash(cQString &key, cQString &salt) const;

    Q_DISABLE_COPY(RunGuard)
};

} // namespace
//-------------------------------------------------------------------------------------------------


namespace qtlib
{

/**************************************************************************************************
*   public
*
**************************************************************************************************/

//-------------------------------------------------------------------------------------------------
Application::Application(
    int       &a_argc,
    char     **a_argv,
    cQString  &a_guid
) :
    QApplication(a_argc, a_argv),
    _guid       (a_guid)
#if !defined(Q_OS_ANDROID)
    ,
    _locker     (a_guid)
#endif
{
    qTEST(!a_guid.isEmpty());

    // set codecs
    {
        QTextCodec *codec = QTextCodec::codecForName(::localeCodec);
        qTEST_PTR(codec);

        QTextCodec::setCodecForLocale(codec);
    }

#if !defined(Q_OS_ANDROID)
    // when can create it only if it doesn't exist
    if ( _locker.create(5000) ) {
        _locker.lock();
        {
            *(char *)_locker.data() = '\0';
        }
        _locker.unlock();

        // _is_running = false;

        // start checking for messages of other instances
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout,
                this,  &Application::checkForMessage);
        timer->start(200);
    }
    // it exits, so we can attach it
    else if ( _locker.attach() ){
        // _is_running = true;
    }
    else {
        // error
    }
#endif
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
#if !defined(Q_OS_ANDROID)
    static ::RunGuard guard(_guid);

    return !guard.tryToRun();
#else
    // TODO: [Android] Application::isRunnig()
    return false;
#endif
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
#if !defined(Q_OS_ANDROID)
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
#else
    Q_UNUSED(a_message);
#endif

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
    bool bRv {};

    bRv = (QString(QT_VERSION_STR) == QString( qVersion() ));
    if (!bRv) {
        qDebug()
            << "QtLib/Application: "
            << "Qt compile version \"" << QT_VERSION_STR << "\" and "
            << "Qt runtime version \"" << qVersion()     << "\" mismatch";
        return false;
    }

#if 0
    // TODO: Application::selfCheck()

    bRv = QDir( pluginPlatformsDirPath() ).isReadable();
    if (!bRv) {
        qDebug()
            << "QtLib/Application: "
            << "Missing plugin directory \""
            << pluginPlatformsDirPath() << "\"";
        return false;
    }

    bRv = QDir( pluginSqlDriversDirPath() ).isReadable();
    if (!bRv) {
        qDebug()
            << "QtLib/Application: "
            << "Missing plugin directory \""
            << pluginSqlDriversDirPath() << "\"";
        return false;
    }

    bRv = QDir( pluginImageFormatsDirPath() ).isReadable();
    if (!bRv) {
        qDebug()
            << "QtLib/Application: "
            << "Missing plugin directory \""
            << pluginImageFormatsDirPath() << "\"";
        return false;
    }
#endif

    return true;
}
//-------------------------------------------------------------------------------------------------
/* static */
void
Application::restart()
{
    QStringList args = QApplication::arguments();
    args.removeFirst();

    QProcess::startDetached(QApplication::applicationFilePath(), args);

    QCoreApplication::quit();
}
//-------------------------------------------------------------------------------------------------
void
Application::checkForMessage()
{
#if !defined(Q_OS_ANDROID)
    QStringList arguments;

    _locker.lock();
    {
        char *from = static_cast<char *>( _locker.data() );

        while (*from != '\0') {
            cint sizeToRead = *from;

            ++ from;

            QByteArray buff = QByteArray(from, sizeToRead);
            buff[sizeToRead] = '\0';

            from += sizeToRead;

            arguments << QString::fromUtf8( buff.constData() );
        }

        *static_cast<char *>( _locker.data() ) = '\0';
    }
    _locker.unlock();

    if ( !arguments.isEmpty() ) {
        Q_EMIT sig_messageAvailable(arguments);
    }
#endif
}
//-------------------------------------------------------------------------------------------------

} // namespace qtlib


namespace
{

#include <QObject>
#include <QSharedMemory>
#include <QSystemSemaphore>
#include <QCryptographicHash>

//-------------------------------------------------------------------------------------------------
RunGuard::RunGuard(
    cQString &a_key
) :
    _key         (a_key),
    _memLockKey  (_generateKeyHash(a_key, "_memLockKey" )),
    _sharedmemKey(_generateKeyHash(a_key, "_sharedmemKey" )),
    _sharedMem   (_sharedmemKey),
    _memLock     (_memLockKey, 1)
{
    QSharedMemory fix(_sharedmemKey);   // Fix for *nix: http://habrahabr.ru/post/173281/
    fix.attach();
}
//-------------------------------------------------------------------------------------------------
RunGuard::~RunGuard()
{
    release();
}
//-------------------------------------------------------------------------------------------------
bool
RunGuard::isAnotherRunning()
{
    if ( _sharedMem.isAttached() ) {
        return false;
    }

    _memLock.acquire();

    cbool isRunning = _sharedMem.attach();
    if (isRunning) {
        _sharedMem.detach();
    }

    _memLock.release();

    return isRunning;
}
//-------------------------------------------------------------------------------------------------
bool
RunGuard::tryToRun()
{
    if ( isAnotherRunning() ) {   // Extra check
        return false;
    }

    _memLock.acquire();

    cbool result = _sharedMem.create( sizeof( quint64 ) );

    _memLock.release();

    if ( !result ) {
        release();

        return false;
    }

    return true;
}
//-------------------------------------------------------------------------------------------------
void
RunGuard::release()
{
    _memLock.acquire();
    {
        if ( _sharedMem.isAttached() ) {
            _sharedMem.detach();
        }
    }
    _memLock.release();
}
//-------------------------------------------------------------------------------------------------
QString
RunGuard::_generateKeyHash(
    cQString &a_key,
    cQString &a_salt
) const
{
    QByteArray data;
    data.append( a_key.toUtf8() );
    data.append( a_salt.toUtf8() );

    data = QCryptographicHash::hash(data, QCryptographicHash::Sha1).toHex();

    return data;
}
//-------------------------------------------------------------------------------------------------

} // namespace
