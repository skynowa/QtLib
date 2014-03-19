/**
 * \file   CGlobalShortcut_x11.cpp
 * \brief  Global shortcut
 */


#include "CGlobalShortcut_x11.h"


/**************************************************************************************************
*   public
*
**************************************************************************************************/

namespace {
    #if defined(KeyPress)
        const int XKeyPress = KeyPress;
        #undef KeyPress
    #endif

    #if defined(KeyRelease)
        const int XKeyRelease = KeyRelease;
        #undef KeyRelease
    #endif
}

//-------------------------------------------------------------------------------------------------
CGlobalShortcut_x11::CGlobalShortcut_x11(
    QWidget *a_parent
) :
    QMainWindow(a_parent),
    _display   (Q_NULLPTR)
{
    qApp->installEventFilter(this);

    _display = ::XOpenDisplay(Q_NULLPTR);

    _keyCode = ::XKeysymToKeycode(_display, XK_F11);
    ::XGrabKey(_display, _keyCode, ControlMask | ShiftMask, ::XDefaultRootWindow(_display),
        False, GrabModeAsync, GrabModeAsync);
    ::XFlush(_display);
}
//-------------------------------------------------------------------------------------------------
/* virtual */
CGlobalShortcut_x11::~CGlobalShortcut_x11()
{
    ::XUngrabKey(_display, _keyCode, ControlMask | ShiftMask, ::XDefaultRootWindow(_display));
    ::XCloseDisplay(_display);  _display = Q_NULLPTR;
}
//-------------------------------------------------------------------------------------------------
/* virtual */
bool
CGlobalShortcut_x11::eventFilter(
    QObject *a_object,
    QEvent  *a_event
)
{
    Q_UNUSED(a_object);

    if (a_event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(a_event);

        if (keyEvent->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier) &&
            keyEvent->key() == Qt::Key_F11)
        {
            qDebug() << "CGlobalShortcut_x11 event";
            return false;
        }
    }

    return true;
}
//-------------------------------------------------------------------------------------------------


/**************************************************************************************************
*   private
*
**************************************************************************************************/
