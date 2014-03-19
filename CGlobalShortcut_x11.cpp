/**
 * \file   CGlobalShortcut_x11.cpp
 * \brief  Global shortcut
 */


#include "CGlobalShortcut_x11.h"

#if defined(Q_OS_WIN)
    #include <windows.h>
#elif defined(Q_OS_UNIX)
    #include <QX11Info>

    #include <X11/X.h>
    #include <X11/Xlib.h>
    #include <X11/keysym.h>

    #if defined(KeyPress)
        const int XKeyPress   = KeyPress;
        const int XKeyRelease = KeyRelease;

        #undef KeyPress
        #undef KeyRelease
    #endif
#endif


/**************************************************************************************************
*   public
*
**************************************************************************************************/

//-------------------------------------------------------------------------------------------------
CGlobalShortcut_x11::CGlobalShortcut_x11(
    QWidget *a_parent
) :
    QMainWindow(a_parent)
{
    qApp->installEventFilter(this);

    m_keyCode = ::XKeysymToKeycode(QX11Info::display(), XK_F11);
    ::XGrabKey(QX11Info::display(), m_keyCode, ControlMask | ShiftMask, QX11Info::appRootWindow(),
        False, GrabModeAsync, GrabModeAsync);
    ::XFlush(QX11Info::display());
}
//-------------------------------------------------------------------------------------------------
/* virtual */
CGlobalShortcut_x11::~CGlobalShortcut_x11()
{
    ::XUngrabKey(QX11Info::display(), m_keyCode, ControlMask|ShiftMask, QX11Info::appRootWindow());
}
//-------------------------------------------------------------------------------------------------
bool
CGlobalShortcut_x11::eventFilter(
    QObject *a_object,
    QEvent  *a_event
)
{
    Q_UNUSED(a_object);

    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if (keyEvent->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier) &&
            keyEvent->key() == Qt::Key_F11)
        {
            qDebug() << "Ctrl+Shift+F11 pressed!\n";
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
