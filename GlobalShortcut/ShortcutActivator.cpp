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
    auto *display = static_cast<Display *>(this->display);

    const Window       grab_window   = DefaultRootWindow(display);
    const Bool         owner_events  = False;
    const int          pointer_mode  = GrabModeAsync;
    const int          keyboard_mode = GrabModeAsync;

    ::XGrabKey(display, keycode, modifiers, grab_window, owner_events, pointer_mode, keyboard_mode);
    ::XGrabKey(display, keycode, modifiers | Mod2Mask, grab_window, owner_events, pointer_mode, keyboard_mode);
    ::XGrabKey(display, keycode, modifiers | LockMask, grab_window, owner_events, pointer_mode, keyboard_mode);
    ::XGrabKey(display, keycode, modifiers | LockMask | Mod2Mask, grab_window, owner_events, pointer_mode, keyboard_mode);

    ::XSelectInput(display, grab_window, KeyPressMask);

    for ( ;; ) {
        qDebug() << "XEvent: start";

        XEvent event {};
        ::XNextEvent(display, &event);

        qDebug() << "XEvent: stop";

        switch(event.type) {
        case KeyPress:
            // qDebug() << "XEvent: Q_EMIT sig_activated()";
            Q_EMIT sig_activated(keycode, modifiers);
        default:
            break;
        }

        if (_isTerminate) {
            break;
        }
    }

    ::XUngrabKey(display, keycode, modifiers, grab_window);
    ::XUngrabKey(display, keycode, modifiers | Mod2Mask, grab_window);
    ::XUngrabKey(display, keycode, modifiers | LockMask, grab_window);
    ::XUngrabKey(display, keycode, modifiers | LockMask | Mod2Mask, grab_window);

    ::XCloseDisplay(display);
}
//-------------------------------------------------------------------------------------------------
