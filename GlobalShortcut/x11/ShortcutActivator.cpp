/**
 * \file  ShortcutActivator.cpp
 * \brief
 */

#include "ShortcutActivator.h"

#include <QDebug>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
//-------------------------------------------------------------------------------------------------
void
ShortcutActivator::end()
{
    _isTerminate = true;
}
//-------------------------------------------------------------------------------------------------
void
ShortcutActivator::run() /* override */
{
    qTRACE_FUNC;

    auto *display = static_cast<Display *>(this->display);

    const quint32 keyModifiers[]
    {
        modifiers,
        modifiers | Mod2Mask,
        modifiers | LockMask,
        modifiers | LockMask | Mod2Mask
    };

    const Window grab_window   { DefaultRootWindow(display) };
    const Bool   owner_events  {False};
    const int    pointer_mode  {GrabModeAsync};
    const int    keyboard_mode {GrabModeAsync};

    for (const auto &it_modifier : keyModifiers) {
        ::XGrabKey(display, keycode, it_modifier, grab_window, owner_events, pointer_mode,
            keyboard_mode);
    }

    ::XSelectInput(display, grab_window, KeyPressMask);

    for ( ;; ) {
        XEvent event {};
        ::XNextEvent(display, &event);

        switch(event.type) {
        case KeyPress:
            Q_EMIT sig_activated(keycode, modifiers);
        default:
            break;
        }

        if (_isTerminate) {
            break;
        }
    }

    for (const auto &it_modifier : keyModifiers) {
        ::XUngrabKey(display, keycode, it_modifier, grab_window);
    }

    ::XCloseDisplay(display);
}
//-------------------------------------------------------------------------------------------------
