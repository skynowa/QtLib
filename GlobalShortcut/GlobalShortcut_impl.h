/**
 * \file
 * \brief
 */


#pragma once

//-------------------------------------------------------------------------------------------------
#include <QAbstractEventDispatcher>
#include <QKeySequence>
#include <QHash>
#include <QAbstractNativeEventFilter>

#include "GlobalShortcut.h"
#include "x11/QxtX11Data.h"
//-------------------------------------------------------------------------------------------------
namespace qtlib
{

class GlobalShortcut_impl :
    public Private<GlobalShortcut>
#if !defined(Q_OS_DARWIN)
    , public QAbstractNativeEventFilter
#endif
{
public:
    QTLIB_DECLARE_PUBLIC(GlobalShortcut)

             GlobalShortcut_impl();
    virtual ~GlobalShortcut_impl();

    bool    enabled {};
    Qt::Key key {};
    Qt::KeyboardModifiers mods {};

    bool setShortcut(const QKeySequence &shortcut);
    bool unsetShortcut();

#if !defined(Q_OS_DARWIN)
    static int ref;

    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) override;
#endif

    void activateShortcut(quint32 nativeKey, quint32 nativeMods);

private:
    static QHash<QPair<quint32, quint32>, GlobalShortcut *> _shortcuts;

    quint32 nativeKeycode(Qt::Key keycode);
    quint32 nativeModifiers(Qt::KeyboardModifiers modifiers);

    bool    _register(quint32 nativeKey, quint32 nativeMods);
    bool    _unregister(quint32 nativeKey, quint32 nativeMods);

private:
#if defined(Q_OS_UNIX)
    QxtX11Data _x11;
#endif
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
