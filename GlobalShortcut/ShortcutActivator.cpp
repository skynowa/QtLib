/**
 * \file  ShortcutActivator.cpp
 * \brief
 */

#include "ShortcutActivator.h"

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
    Display      *display = ::XOpenDisplay(nullptr);
    const Window  root    = DefaultRootWindow(display);

    const unsigned int modifiers     = AnyModifier; // Mod1Mask | ControlMask | ShiftMask | AnyModifier;
    const int          keycode       = ::XKeysymToKeycode(display, XStringToKeysym("F3") /* XK_space */);
    const Window       grab_window   = root;
    const Bool         owner_events  = False;
    const int          pointer_mode  = GrabModeAsync;
    const int          keyboard_mode = GrabModeAsync;

    ::XGrabKey(display, keycode, modifiers, grab_window, owner_events, pointer_mode, keyboard_mode);
    ::XGrabKey(display, keycode, modifiers | Mod2Mask, grab_window, owner_events, pointer_mode, keyboard_mode);
    ::XGrabKey(display, keycode, modifiers | LockMask, grab_window, owner_events, pointer_mode, keyboard_mode);
    ::XGrabKey(display, keycode, modifiers | LockMask | Mod2Mask, grab_window, owner_events, pointer_mode, keyboard_mode);

    ::XSelectInput(display, root, KeyPressMask);

    for ( ;; ) {
        XEvent event {};
        ::XNextEvent(display, &event);

        switch(event.type) {
        case KeyPress:
            printf("Key pressed\n");
            Q_EMIT sig_activated();
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
