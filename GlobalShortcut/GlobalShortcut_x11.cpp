/**
 * \file
 * \brief
 */


#include "GlobalShortcut_impl.h"

#include "x11/ShortcutActivator.h"

namespace qtlib
{

//-------------------------------------------------------------------------------------------------
quint32
GlobalShortcut_impl::_nativeModifiers(
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
GlobalShortcut_impl::_nativeKeycode(
    Qt::Key a_key
)
{
    return _x11.keysymToKeycode(a_key);
}
//-------------------------------------------------------------------------------------------------
bool
GlobalShortcut_impl::_register(
    quint32 a_nativeKey,
    quint32 a_nativeMods
)
{
#if 0
    return _x11.grabKey(a_nativeKey, a_nativeMods);
#else
    // ShortcutActivator
    {
        ShortcutActivator *workerThread = new ShortcutActivator(::XOpenDisplay(nullptr),
            a_nativeKey, a_nativeMods);

        connect(workerThread, &ShortcutActivator::sig_activated,
                this,         &GlobalShortcut_impl::_activateShortcut);
        connect(workerThread, &ShortcutActivator::finished,
                workerThread, &QObject::deleteLater);

        workerThread->start();
    }

    return true;
#endif
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
