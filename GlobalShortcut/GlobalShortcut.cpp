/**
 * \file  GlobalShortcut.cpp
 * \brief
 */


#include "GlobalShortcut.h"

#include "GlobalShortcut_impl.cpp"
#include <QAbstractEventDispatcher>

namespace qtlib
{

//-------------------------------------------------------------------------------------------------
GlobalShortcut::GlobalShortcut(
    QObject *a_parent
) :
    QObject(a_parent)
{
    QTLIB_INIT_PRIVATE(GlobalShortcut);
}
//-------------------------------------------------------------------------------------------------
GlobalShortcut::GlobalShortcut(
    const QKeySequence &a_shortcut,
    QObject            *a_parent
) :
    QObject(a_parent)
{
    QTLIB_INIT_PRIVATE(GlobalShortcut);

    bool bRv = set(a_shortcut);
    qTEST(bRv);
}
//-------------------------------------------------------------------------------------------------
GlobalShortcut::~GlobalShortcut()
{
    if (_impl().key != keyUnknown) {
        bool bRv = _impl().unsetShortcut();
        qTEST(bRv);
    }
}
//-------------------------------------------------------------------------------------------------
/**
 * \property GlobalShortcut::shortcut
 * \brief    the shortcut key sequence
 *
 * \bold {Note:} Notice that corresponding key press and release events are not
 * delivered for registered global shortcuts even if they are disabled.
 * Also, comma separated key sequences are not supported.
 * Only the first part is used:
 *
 * \code
 * qxtShortcut->setShortcut(QKeySequence("Ctrl+Alt+A,Ctrl+Alt+B"));
 * qTEST(qxtShortcut->shortcut() == QKeySequence("Ctrl+Alt+A"));
 * \endcode
 */
QKeySequence
GlobalShortcut::get() const
{
    return QKeySequence(_impl().key | _impl().mods);
}
//-------------------------------------------------------------------------------------------------
bool
GlobalShortcut::set(
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
void
GlobalShortcut::setDisabled(
    bool a_disabled
)
{
    _impl().enabled = !a_disabled;
}
//-------------------------------------------------------------------------------------------------

} // namespace qtlib
