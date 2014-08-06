/**
 * \file   SignalGuard.h
 * \brief
 */


#ifndef QtLib_SignalGuardH
#define QtLib_SignalGuardH
//-------------------------------------------------------------------------------------------------
#include "Common.h"
//-------------------------------------------------------------------------------------------------
namespace qtlib {

class SignalGuard :
    public QObject
    /// Ensures through RAII that a signal will be emitted in the case of early returns
{
public:
         SignalGuard(QObject *target, const char *slot);
        ~SignalGuard();

    void setEmit(const bool &a_flag);

Q_SIGNALS:
    void trigger();

private:
    bool _isEmit;

    Q_OBJECT
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
#endif // QtLib_SignalGuardH


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
 * emit uiWait();
 * foo();
 * bar();
 */