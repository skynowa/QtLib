/**
 * \file
 * \brief
 */


#include "GlobalShortcut.h"

#include "GlobalShortcut_impl.h"
#include <QAbstractEventDispatcher>

#if    defined(Q_OS_WIN)
    #include "GlobalShortcut_win.cpp"
#elif defined(Q_OS_UNIX)
    #include "GlobalShortcut_x11.cpp"
#elif defined(Q_OS_DARWIN)
    #include "GlobalShortcut_apple.cpp"
#else

#endif

namespace qtlib
{

//-------------------------------------------------------------------------------------------------
namespace
{

const Qt::Key               keyUnknown  = static_cast<Qt::Key>(0);
const Qt::KeyboardModifiers modsUnknown = static_cast<Qt::KeyboardModifiers>(0);

}
//-------------------------------------------------------------------------------------------------
#if !defined(Q_OS_DARWIN)
    int GlobalShortcut_impl::ref = 0;
#endif

QHash<QPair<quint32, quint32>, GlobalShortcut *> GlobalShortcut_impl::_shortcuts;
//-------------------------------------------------------------------------------------------------
GlobalShortcut_impl::GlobalShortcut_impl() :
    enabled(true),
    key    (keyUnknown),
    mods   (modsUnknown)
{
#if !defined(Q_OS_DARWIN)
    if (ref == 0) {
        QAbstractEventDispatcher::instance()->installNativeEventFilter(this);
    }

    ++ ref;
#endif
}
//-------------------------------------------------------------------------------------------------
GlobalShortcut_impl::~GlobalShortcut_impl()
{
#if !defined(Q_OS_DARWIN)
    -- ref;

    if (ref == 0) {
        QAbstractEventDispatcher *eventDispatcher = QAbstractEventDispatcher::instance();
        if (eventDispatcher != Q_NULLPTR) {
            eventDispatcher->removeNativeEventFilter(this);
        }
    }
#endif
}
//-------------------------------------------------------------------------------------------------
bool
GlobalShortcut_impl::setShortcut(
    const QKeySequence &a_shortcut
)
{
    const Qt::KeyboardModifiers allMods = Qt::ShiftModifier | Qt::ControlModifier |
                                          Qt::AltModifier   | Qt::MetaModifier;

    key  = a_shortcut.isEmpty() ? keyUnknown  : Qt::Key((a_shortcut[0] ^ allMods) & a_shortcut[0]);
    mods = a_shortcut.isEmpty() ? modsUnknown : Qt::KeyboardModifiers(a_shortcut[0] & allMods);

    const quint32 nativeKey  = nativeKeycode(key);
    const quint32 nativeMods = nativeModifiers(mods);

    const bool bRv = registerShortcut(nativeKey, nativeMods);
    if (bRv) {
        _shortcuts.insert(qMakePair(nativeKey, nativeMods), &get());
    } else {
        qWarning() << "GlobalShortcut failed to register:" << QKeySequence(key + mods).toString();
    }

    return bRv;
}
//-------------------------------------------------------------------------------------------------
bool
GlobalShortcut_impl::unsetShortcut()
{
    bool bRv = false;

    const quint32 nativeKey  = nativeKeycode(key);
    const quint32 nativeMods = nativeModifiers(mods);

    if (_shortcuts.value(qMakePair(nativeKey, nativeMods)) == &get()) {
        bRv = unregisterShortcut(nativeKey, nativeMods);
    }

    if (bRv) {
        _shortcuts.remove(qMakePair(nativeKey, nativeMods));
    } else {
        qWarning() << "GlobalShortcut failed to unregister:" << QKeySequence(key + mods).toString();
    }

    key  = keyUnknown;
    mods = modsUnknown;

    return bRv;
}
//-------------------------------------------------------------------------------------------------
void
GlobalShortcut_impl::activateShortcut(
    quint32 a_nativeKey,
    quint32 a_nativeMods
)
{
    GlobalShortcut *shortcut = _shortcuts.value(qMakePair(a_nativeKey, a_nativeMods));
    if (shortcut != Q_NULLPTR && shortcut->isEnabled()) {
        Q_EMIT shortcut->sig_activated();
    }
}
//-------------------------------------------------------------------------------------------------

} // namespace qtlib