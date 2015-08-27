/**
 * \file   RunGuard.cpp
 * \brief
 */


#include "RunGuard.h"

#include <QCryptographicHash>


namespace qtlib
{

//-------------------------------------------------------------------------------------------------
RunGuard::RunGuard(
    const QString &a_key
) :
    _key         (a_key),
    _memLockKey  (_generateKeyHash(a_key, "_memLockKey" )),
    _sharedmemKey(_generateKeyHash(a_key, "_sharedmemKey" )),
    _sharedMem   (_sharedmemKey),
    _memLock     (_memLockKey, 1)
{
    QSharedMemory fix(_sharedmemKey);    // Fix for *nix: http://habrahabr.ru/post/173281/
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

    const bool isRunning = _sharedMem.attach();
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

    const bool result = _sharedMem.create( sizeof( quint64 ) );

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
    const QString &a_key,
    const QString &a_salt
) const
{
    QByteArray data;
    data.append( a_key.toUtf8() );
    data.append( a_salt.toUtf8() );

    data = QCryptographicHash::hash(data, QCryptographicHash::Sha1).toHex();

    return data;
}
//-------------------------------------------------------------------------------------------------

} // namespace qtlib
