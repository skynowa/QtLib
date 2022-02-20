/**
 * \file  SignalGuard.h
 * \brief
 */


#pragma once

#include "Common.h"
//-------------------------------------------------------------------------------------------------
namespace qtlib
{

class SignalGuard :
    public QObject
    /// Ensures through RAII that a signal will be emitted in the case of early returns
{
public:
///@name ctors, dtor
///@{
    SignalGuard(QObject *target, cchar *slot);
    ~SignalGuard();

    Q_DISABLE_COPY(SignalGuard);
///@}

    void setEmit(cbool a_flag);

Q_SIGNALS:
    void sig_trigger();

private:
    Q_OBJECT

    bool _isEmit {true};
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------


/**
 * Sample 1:
 *
 * SignalGuard cancelGuard(this, SIGNAL(requestCanceled()));
 * // do check one, if failed show a message, return
 * // do check two, if failed show a message, return
 * // do check three, if failed show a message, return
 * cancelGuard.dismiss(); // phew, we've made it
 *
 *
 * Sample 2:
 *
 * SignalGuard g(this, SIGNAL(uiFinishWait()));
 * Q_EMIT uiWait();
 * foo();
 * bar();
 */
