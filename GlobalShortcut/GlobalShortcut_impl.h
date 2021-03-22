/**
 * \file  GlobalShortcut_impl.h
 * \brief
 */


#pragma once

//-------------------------------------------------------------------------------------------------
#include <QAbstractEventDispatcher>
#include <QKeySequence>
#include <QHash>
#include <QAbstractNativeEventFilter>

#include "GlobalShortcut.h"
#include "x11/X11Data.h"
//-------------------------------------------------------------------------------------------------
namespace qtlib
{

class GlobalShortcut_impl :
    public Private<GlobalShortcut>,
    public QObject
{
public:
             GlobalShortcut_impl();
    virtual ~GlobalShortcut_impl();

    bool setShortcut(const QKeySequence &shortcut);
    bool unsetShortcut();

    QTLIB_DECLARE_PUBLIC(GlobalShortcut)

private:
    bool                  enabled {};
    Qt::Key               key {};
    Qt::KeyboardModifiers mods {};

#if defined(Q_OS_UNIX)
    X11Data _x11;
#endif

    static QHash<QPair<quint32, quint32>, GlobalShortcut *> _shortcuts;

    quint32 _nativeKeycode(Qt::Key keycode);
    quint32 _nativeModifiers(Qt::KeyboardModifiers modifiers);

    bool    _register(quint32 nativeKey, quint32 nativeMods);
    bool    _unregister(quint32 nativeKey, quint32 nativeMods);

    void    _activateShortcut(quint32 nativeKey, quint32 nativeMods);
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
