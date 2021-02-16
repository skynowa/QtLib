/**
 * \file
 * \brief
 */


#include "GlobalShortcut_impl.h"


namespace qtlib
{

//-------------------------------------------------------------------------------------------------
bool
GlobalShortcut_impl::nativeEventFilter(
    const QByteArray &a_eventType,
    void             *a_message,
    long             *a_result
) /* override */
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
    KeySym keysym = ::XStringToKeysym(QKeySequence(a_key).toString().toLatin1().data());
    if (keysym == NoSymbol) {
        qTEST(false);
        keysym = static_cast<ushort>(a_key);
    }

    return _x11.keysymToKeycode(keysym);
}
//-------------------------------------------------------------------------------------------------
bool
GlobalShortcut_impl::_register(
    quint32 a_nativeKey,
    quint32 a_nativeMods
)
{
    return _x11.grabKey(a_nativeKey, a_nativeMods);
}
//-------------------------------------------------------------------------------------------------
bool
GlobalShortcut_impl::_unregister(
    quint32 a_nativeKey,
    quint32 a_nativeMods
)
{
    return _x11.ungrabKey(a_nativeKey, a_nativeMods);
}
//-------------------------------------------------------------------------------------------------

} // namespace qtlib
