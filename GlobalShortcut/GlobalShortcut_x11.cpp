/**
 * \file
 * \brief
 */


#include "GlobalShortcut_impl.h"

// Qt 5.11.1: don't supprt Qt Platform Abstraction (QPA)
#define QT_QPA 0

#if QT_QPA
#include <qpa/qplatformnativeinterface.h>
#endif

#include <X11/Xlib.h>
#include <xcb/xcb.h>
#include <QProcessEnvironment>

#include "x11/QxtX11ErrorHandler.h"
#include "x11/QxtX11Data.h"

namespace qtlib
{

//-------------------------------------------------------------------------------------------------
bool
GlobalShortcut_impl::nativeEventFilter(
    const QByteArray &a_eventType,
    void             *a_message,
    long             *a_result
)
{
    qTEST(a_eventType.size() > 0);
    qTEST_PTR(a_message);
    qTEST_PTR(a_result);

    Q_UNUSED(a_result);

    xcb_key_press_event_t *kev {};

    // qDebug() << qTRACE_VAR(a_eventType);

    if (a_eventType == "xcb_generic_event_t") {
        xcb_generic_event_t *event = static_cast<xcb_generic_event_t *>(a_message);
        // qDebug() << qTRACE_VAR(event->response_type);
        if ((event->response_type & 127) == XCB_KEY_PRESS) {
            kev = static_cast<xcb_key_press_event_t *>(a_message);
        }
    }

    if (kev != Q_NULLPTR) {
        unsigned int keycode  = kev->detail;
        unsigned int keystate = 0;

        if (kev->state & XCB_MOD_MASK_1) {
            keystate |= Mod1Mask;
        }
        if (kev->state & XCB_MOD_MASK_CONTROL) {
            keystate |= ControlMask;
        }
        if (kev->state & XCB_MOD_MASK_4) {
            keystate |= Mod4Mask;
        }
        if (kev->state & XCB_MOD_MASK_SHIFT) {
            keystate |= ShiftMask;
        }

        activateShortcut(keycode,
            // Mod1Mask == Alt, Mod4Mask == Meta
            keystate & (ShiftMask | ControlMask | Mod1Mask | Mod4Mask));
    }

    return false;
}
//-------------------------------------------------------------------------------------------------
quint32
GlobalShortcut_impl::nativeModifiers(
    Qt::KeyboardModifiers a_modifiers
)
{
    // ShiftMask, LockMask, ControlMask, Mod1Mask, Mod2Mask, Mod3Mask, Mod4Mask, and Mod5Mask
    quint32 native {};

    if (a_modifiers & Qt::ShiftModifier) {
        native |= ShiftMask;
    }
    if (a_modifiers & Qt::ControlModifier) {
        native |= ControlMask;
    }
    if (a_modifiers & Qt::AltModifier) {
        native |= Mod1Mask;
    }
    if (a_modifiers & Qt::MetaModifier) {
        native |= Mod4Mask;
    }

    // TODO: resolve these?
    // if (modifiers & Qt::MetaModifier)
    // if (modifiers & Qt::KeypadModifier)
    // if (modifiers & Qt::GroupSwitchModifier)

    return native;
}
//-------------------------------------------------------------------------------------------------
quint32
GlobalShortcut_impl::nativeKeycode(
    Qt::Key a_key
)
{
    QxtX11Data x11;
    if ( !x11.isValid() ) {
        return {};
    }

    KeySym keysym = ::XStringToKeysym(QKeySequence(a_key).toString().toLatin1().data());
    if (keysym == NoSymbol) {
        qTEST(false);
        keysym = static_cast<ushort>(a_key);
    }

    return ::XKeysymToKeycode(x11.display(), keysym);
}
//-------------------------------------------------------------------------------------------------
bool
GlobalShortcut_impl::registerShortcut(
    quint32 a_nativeKey,
    quint32 a_nativeMods
)
{
    /// qTEST(a_nativeKey > 0);
    /// qTEST(a_nativeMods > 0);

    QxtX11Data x11;

    if ( !x11.isValid() ) {
        qDebug() << qTRACE_VAR(__FUNCTION__) << ": " << qTRACE_VAR(__LINE__) << " - FAIL";
        return false;
    }

    if ( !x11.grabKey(a_nativeKey, a_nativeMods, x11.rootWindow()) ) {
        qDebug() << qTRACE_VAR(__FUNCTION__) << ": " << qTRACE_VAR(__LINE__) << " - FAIL";
        return false;
    }

    qDebug() << qTRACE_VAR(__FUNCTION__) << ": " << qTRACE_VAR(__LINE__) << " - OK";

    return true;
}
//-------------------------------------------------------------------------------------------------
bool
GlobalShortcut_impl::unregisterShortcut(
    quint32 a_nativeKey,
    quint32 a_nativeMods
)
{
    QxtX11Data x11;

    if ( !x11.isValid() ) {
        qDebug() << qTRACE_VAR(__FUNCTION__) << ": " << qTRACE_VAR(__LINE__) << " - FAIL";
        return false;
    }

    if ( !x11.ungrabKey(a_nativeKey, a_nativeMods, x11.rootWindow()) ) {
        qDebug() << qTRACE_VAR(__FUNCTION__) << ": " << qTRACE_VAR(__LINE__) << " - FAIL";
        return false;
    }

    qDebug() << qTRACE_VAR(__FUNCTION__) << ": " << qTRACE_VAR(__LINE__) << " - OK";

    return true;
}
//-------------------------------------------------------------------------------------------------

} // namespace qtlib
