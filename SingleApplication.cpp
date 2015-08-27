/**
 * \file   SingleApplication.cpp
 * \brief
 */


#include "SingleApplication.h"

#include <QTimer>
#include <QByteArray>


namespace qtlib
{

//-------------------------------------------------------------------------------------------------
SingleApplication::SingleApplication(
    int      &a_argc,
    char     *a_argv[],
    cQString &a_guid
) :
    QApplication(a_argc, a_argv),
    _is_running (false),
    _locker     (a_guid)
{
    // when can create it only if it doesn't exist
    if ( _locker.create(5000) ) {
        _locker.lock();
        {
            *(char*)_locker.data() = '\0';
        }
        _locker.unlock();

        _is_running = false;

        // start checking for messages of other instances
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(checkForMessage()));
        timer->start(200);
    }
    // it exits, so we can attach it
    else if ( _locker.attach() ){
        _is_running = true;
    }
    else {
        // error
    }
}
//-------------------------------------------------------------------------------------------------
bool
SingleApplication::isRunnig() const
{
    return _is_running;
}
//-------------------------------------------------------------------------------------------------
bool
SingleApplication::isMaster() const
{
    return !isRunnig();
}
//-------------------------------------------------------------------------------------------------
// public functions
bool
SingleApplication::sendMessage(
    const QString &a_message
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
// public slots
void
SingleApplication::checkForMessage()
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
