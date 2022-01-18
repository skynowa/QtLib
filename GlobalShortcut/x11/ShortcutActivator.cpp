/**
 * \file  ShortcutActivator.cpp
 * \brief
 */

#include "ShortcutActivator.h"

#include <QDebug>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
//-------------------------------------------------------------------------------------------------
ShortcutActivator::ShortcutActivator(
    void          *a_display,
    const quint32  a_keycode,
    const quint32  a_modifiers
) :
    _display  {a_display},
    _keycode  {a_keycode},
    _modifiers{a_modifiers}
{
    qTEST_PTR(a_display);
    qTEST(_keycode > 0);
    qTEST_NA(_modifiers);
}
//-------------------------------------------------------------------------------------------------
void
ShortcutActivator::end()
{
    _isTerminate = true;
}
//-------------------------------------------------------------------------------------------------
void
ShortcutActivator::run() /* final */
{
    // qTRACE_FUNC << "- start";

    auto *display = static_cast<Display *>(_display);

    const quint32 keyModifiers[]
    {
        _modifiers,
        _modifiers | Mod2Mask,
        _modifiers | LockMask,
        _modifiers | LockMask | Mod2Mask
    };

    const Window grab_window   { DefaultRootWindow(display) };
    const Bool   owner_events  {True};
    const int    pointer_mode  {GrabModeAsync};
    const int    keyboard_mode {GrabModeAsync};

    for (const auto it_modifier : keyModifiers) {
        ::XGrabKey(display, _keycode, it_modifier, grab_window, owner_events, pointer_mode,
            keyboard_mode);
    }

    ::XSelectInput(display, grab_window, KeyPressMask);

    for ( ;; ) {
        // qDebug() << "XNextEvent - start";

        XEvent event {};
        ::XNextEvent(display, &event);

        // qDebug() << "XNextEvent - stop";

        switch(event.type) {
        case KeyPress:
            Q_EMIT sig_activated(_keycode, _modifiers);
        default:
            break;
        }

        if (_isTerminate) {
            break;
        }
    }

    for (const auto it_modifier : keyModifiers) {
        ::XUngrabKey(display, _keycode, it_modifier, grab_window);
    }

    ::XCloseDisplay(display);

    // qTRACE_FUNC << "- stop";
}
//-------------------------------------------------------------------------------------------------
