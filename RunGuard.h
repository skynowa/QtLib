/**
 * \file   RunGuard.h
 * \brief
 */


#pragma once

#include <QObject>
#include <QSharedMemory>
#include <QSystemSemaphore>
//-------------------------------------------------------------------------------------------------
namespace qtlib
{

class RunGuard
{
public:
    RunGuard(const QString &key);
    ~RunGuard();

    bool isAnotherRunning();
    bool tryToRun();
    void release();

private:
    const QString    _key;
    const QString    _memLockKey;
    const QString    _sharedmemKey;

    QSharedMemory    _sharedMem;
    QSystemSemaphore _memLock;

    QString _generateKeyHash(const QString &key, const QString &salt) const;

    Q_DISABLE_COPY(RunGuard)
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
