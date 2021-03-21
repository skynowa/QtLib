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
    qTRACE_FUNC << "- start";

    auto *display = static_cast<Display *>(this->display);
    qTEST_PTR(display);
    qTEST(keycode > 0);
    qTEST(modifiers > 0);

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
        ::XGrabKey(display, this->keycode, it_modifier, grab_window, owner_events, pointer_mode,
            keyboard_mode);
    }

    ::XSelectInput(display, grab_window, KeyPressMask);

    for ( ;; ) {
        qDebug() << "XNextEvent - start";

        XEvent event {};
        ::XNextEvent(display, &event);

        qDebug() << "XNextEvent - stop";

        switch(event.type) {
        case KeyPress:
            Q_EMIT sig_activated(this->keycode, this->modifiers);
        default:
            break;
        }

        if (_isTerminate) {
            break;
        }
    }

    for (const auto &it_modifier : keyModifiers) {
        ::XUngrabKey(display, this->keycode, it_modifier, grab_window);
    }

    ::XCloseDisplay(display);

    qTRACE_FUNC << "- stop";
}
//-------------------------------------------------------------------------------------------------
