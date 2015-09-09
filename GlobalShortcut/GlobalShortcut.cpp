/**
 * \file
 * \brief
 */


#include "GlobalShortcut.h"

#include "GlobalShortcut_impl.h"
#include <QAbstractEventDispatcher>


namespace qtlib
{

//-------------------------------------------------------------------------------------------------
#ifndef Q_OS_MAC
    int GlobalShortcut_impl::ref = 0;
#endif

QHash<QPair<quint32, quint32>, GlobalShortcut *> GlobalShortcut_impl::shortcuts;
//-------------------------------------------------------------------------------------------------
GlobalShortcut_impl::GlobalShortcut_impl() :
    enabled(true),
    key    (Qt::Key(0)),
    mods   (Qt::NoModifier)
{
#ifndef Q_OS_MAC
    if (ref == 0) {
        QAbstractEventDispatcher::instance()->installNativeEventFilter(this);
    }

    ++ ref;
#endif
}
//-------------------------------------------------------------------------------------------------
GlobalShortcut_impl::~GlobalShortcut_impl()
{
#ifndef Q_OS_MAC
    -- ref;

    if (ref == 0) {
        QAbstractEventDispatcher *ed = QAbstractEventDispatcher::instance();
        if (ed != Q_NULLPTR) {
            ed->removeNativeEventFilter(this);
        }
    }
#endif
}
//-------------------------------------------------------------------------------------------------
bool
GlobalShortcut_impl::setShortcut(
    const QKeySequence &shortcut
)
{
    Qt::KeyboardModifiers allMods = Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier;
    key  = shortcut.isEmpty() ? Qt::Key(0) : Qt::Key((shortcut[0] ^ allMods) & shortcut[0]);
    mods = shortcut.isEmpty() ? Qt::KeyboardModifiers(0) : Qt::KeyboardModifiers(shortcut[0] & allMods);

    const quint32 nativeKey  = nativeKeycode(key);
    const quint32 nativeMods = nativeModifiers(mods);

    const bool res = registerShortcut(nativeKey, nativeMods);
    if (res) {
        shortcuts.insert(qMakePair(nativeKey, nativeMods), &get());
    } else {
        qWarning() << "GlobalShortcut failed to register:" << QKeySequence(key + mods).toString();
    }

    return res;
}
//-------------------------------------------------------------------------------------------------
bool
GlobalShortcut_impl::unsetShortcut()
{
    bool res = false;
    const quint32 nativeKey  = nativeKeycode(key);
    const quint32 nativeMods = nativeModifiers(mods);

    if (shortcuts.value(qMakePair(nativeKey, nativeMods)) == &get()) {
        res = unregisterShortcut(nativeKey, nativeMods);
    }

    if (res) {
        shortcuts.remove(qMakePair(nativeKey, nativeMods));
    } else {
        qWarning() << "GlobalShortcut failed to unregister:" << QKeySequence(key + mods).toString();
    }

    key  = Qt::Key(0);
    mods = Qt::KeyboardModifiers(0);

    return res;
}
//-------------------------------------------------------------------------------------------------
void
GlobalShortcut_impl::activateShortcut(
    quint32 nativeKey,
    quint32 nativeMods
)
{
    GlobalShortcut* shortcut = shortcuts.value(qMakePair(nativeKey, nativeMods));
    if (shortcut != Q_NULLPTR && shortcut->isEnabled()) {
        Q_EMIT shortcut->sig_activated();
    }
}
//-------------------------------------------------------------------------------------------------
/*!
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

/*!
    Constructs a new GlobalShortcut with \a parent.
 */
GlobalShortcut::GlobalShortcut(
    QObject *parent
) :
    QObject(parent)
{
    QTLIB_INIT_PRIVATE(GlobalShortcut);
}
//-------------------------------------------------------------------------------------------------
/*!
    Constructs a new GlobalShortcut with \a shortcut and \a parent.
 */
GlobalShortcut::GlobalShortcut(
    const QKeySequence &shortcut,
    QObject            *parent
) :
    QObject(parent)
{
    QTLIB_INIT_PRIVATE(GlobalShortcut);

    setShortcut(shortcut);
}
//-------------------------------------------------------------------------------------------------
/*!
    Destructs the GlobalShortcut.
 */
GlobalShortcut::~GlobalShortcut()
{
    if (_impl().key != 0) {
        _impl().unsetShortcut();
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
    Q_ASSERT(qxtShortcut->shortcut() == QKeySequence("Ctrl+Alt+A"));
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
    const QKeySequence& shortcut
)
{
    if (_impl().key != 0) {
        _impl().unsetShortcut();
    }

    return _impl().setShortcut(shortcut);
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
    bool enabled
)
{
    _impl().enabled = enabled;
}
//-------------------------------------------------------------------------------------------------
/*!
    Sets the shortcut \a disabled.

    \sa enabled
 */
void
GlobalShortcut::setDisabled(
    bool disabled
)
{
    _impl().enabled = !disabled;
}
//-------------------------------------------------------------------------------------------------

} // namespace qtlib
