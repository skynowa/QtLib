/**
 * \file  GlobalShortcut_impl.cpp
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

const Qt::Key               keyUnknown  {Qt::Key_unknown};
const Qt::KeyboardModifiers modsUnknown {Qt::NoModifier};

}
//-------------------------------------------------------------------------------------------------
QHash<QPair<quint32, quint32>, GlobalShortcut *> GlobalShortcut_impl::_shortcuts;
//-------------------------------------------------------------------------------------------------
GlobalShortcut_impl::GlobalShortcut_impl() :
    enabled{true},
    key    {keyUnknown},
    mods   {modsUnknown}
{
}
//-------------------------------------------------------------------------------------------------
GlobalShortcut_impl::~GlobalShortcut_impl()
{
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

    const quint32 nativeKey  = _nativeKeycode(key);
    const quint32 nativeMods = _nativeModifiers(mods);

    const bool bRv = _register(nativeKey, nativeMods);
    if (bRv) {
        _shortcuts.insert({nativeKey, nativeMods}, &get());
    } else {
        qWarning() << "GlobalShortcut failed to register:" << QKeySequence(key + mods).toString();
    }

    return bRv;
}
//-------------------------------------------------------------------------------------------------
bool
GlobalShortcut_impl::unsetShortcut()
{
    bool bRv {};

    const quint32 nativeKey  = _nativeKeycode(key);
    const quint32 nativeMods = _nativeModifiers(mods);

    if (_shortcuts.value( {nativeKey, nativeMods} ) == &get()) {
        // qDebug() << "_unregister()...";
        bRv = _unregister(nativeKey, nativeMods);
    }

    if (bRv) {
        // qDebug() << "_shortcuts.remove()...";
        _shortcuts.remove( {nativeKey, nativeMods} );
    } else {
        qWarning() << "GlobalShortcut failed to unregister:" << QKeySequence(key + mods).toString();
    }

    key  = keyUnknown;
    mods = modsUnknown;

    return bRv;
}
//-------------------------------------------------------------------------------------------------
void
GlobalShortcut_impl::_activateShortcut(
    quint32 a_nativeKey,
    quint32 a_nativeMods
)
{
    GlobalShortcut *shortcut = _shortcuts.value( {a_nativeKey, a_nativeMods} );
    if (shortcut != Q_NULLPTR &&
        shortcut->isEnabled())
    {
        Q_EMIT shortcut->sig_activated();
    }
}
//-------------------------------------------------------------------------------------------------

} // namespace qtlib
