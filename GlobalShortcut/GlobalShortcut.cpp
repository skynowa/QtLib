/*!
    \file
    \class GlobalShortcut
    \inmodule QxtWidgets
    \brief The GlobalShortcut class provides a global shortcut aka "hotkey".

    A global shortcut triggers even if the application is not active. This
    makes it easy to implement applications that react to certain shortcuts
    still if some other application is active or if the application is for
    example minimized to the system tray.

    Example usage:
    \code
    GlobalShortcut* shortcut = new GlobalShortcut(window);
    connect(shortcut, SIGNAL(activated()), window, SLOT(toggleVisibility()));
    shortcut->setShortcut(QKeySequence("Ctrl+Shift+F12"));
    \endcode

    \bold {Note:} Since Qxt 0.6 GlobalShortcut no more requires QxtApplication.
 */

/*!
    \fn GlobalShortcut::activated()

    This signal is emitted when the user types the shortcut's key sequence.

    \sa shortcut
 */


#include "GlobalShortcut.h"

#include "GlobalShortcut_impl.cpp"
#include <QAbstractEventDispatcher>

namespace qtlib
{

//-------------------------------------------------------------------------------------------------
/*!
    Constructs a new GlobalShortcut with \a parent.
 */
GlobalShortcut::GlobalShortcut(
    QObject *a_parent
) :
    QObject(a_parent)
{
    QTLIB_INIT_PRIVATE(GlobalShortcut);
}
//-------------------------------------------------------------------------------------------------
/*!
    Constructs a new GlobalShortcut with \a shortcut and \a parent.
 */
GlobalShortcut::GlobalShortcut(
    const QKeySequence &a_shortcut,
    QObject            *a_parent
) :
    QObject(a_parent)
{
    QTLIB_INIT_PRIVATE(GlobalShortcut);

    bool bRv = setShortcut(a_shortcut);
    qTEST(bRv);
}
//-------------------------------------------------------------------------------------------------
/*!
    Destructs the GlobalShortcut.
 */
GlobalShortcut::~GlobalShortcut()
{
    if (_impl().key != keyUnknown) {
        bool bRv = _impl().unsetShortcut();
        qTEST(bRv);
    }
}
//-------------------------------------------------------------------------------------------------
/*!
    \property GlobalShortcut::shortcut
    \brief the shortcut key sequence

    \bold {Note:} Notice that corresponding key press and release events are not
    delivered for registered global shortcuts even if they are disabled.
    Also, comma separated key sequences are not supported.
    Only the first part is used:

    \code
    qxtShortcut->setShortcut(QKeySequence("Ctrl+Alt+A,Ctrl+Alt+B"));
    qTEST(qxtShortcut->shortcut() == QKeySequence("Ctrl+Alt+A"));
    \endcode
 */
QKeySequence
GlobalShortcut::shortcut() const
{
    return QKeySequence(_impl().key | _impl().mods);
}
//-------------------------------------------------------------------------------------------------
bool
GlobalShortcut::setShortcut(
    const QKeySequence &a_shortcut
)
{
    if (_impl().key != keyUnknown) {
        bool bRv = _impl().unsetShortcut();
        qTEST(bRv);
    }

    return _impl().setShortcut(a_shortcut);
}
//-------------------------------------------------------------------------------------------------
/*!
    \property GlobalShortcut::enabled
    \brief whether the shortcut is enabled

    A disabled shortcut does not get activated.

    The default value is \c true.

    \sa setDisabled()
 */
bool
GlobalShortcut::isEnabled() const
{
    return _impl().enabled;
}
//-------------------------------------------------------------------------------------------------
void
GlobalShortcut::setEnabled(
    bool a_enabled
)
{
    _impl().enabled = a_enabled;
}
//-------------------------------------------------------------------------------------------------
/*!
    Sets the shortcut \a disabled.

    \sa enabled
 */
void
GlobalShortcut::setDisabled(
    bool a_disabled
)
{
    _impl().enabled = !a_disabled;
}
//-------------------------------------------------------------------------------------------------

} // namespace qtlib
