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
//-------------------------------------------------------------------------------------------------
namespace qtlib
{

class GlobalShortcut_impl :
    public Private<GlobalShortcut>
#if !defined(Q_OS_MAC)
    , public QAbstractNativeEventFilter
#endif
{
public:
    QTLIB_DECLARE_PUBLIC(GlobalShortcut)

                   GlobalShortcut_impl();
    virtual       ~GlobalShortcut_impl();

    bool           enabled;
    Qt::Key        key;
    Qt::KeyboardModifiers mods;

    bool           setShortcut(const QKeySequence &shortcut);
    bool           unsetShortcut();

    static bool    error;

#ifndef Q_OS_MAC
    static int     ref;

    virtual bool   nativeEventFilter(const QByteArray &eventType, void *message, long *result);
#endif

    static void    activateShortcut(quint32 nativeKey, quint32 nativeMods);

private:
    static quint32 nativeKeycode(Qt::Key keycode);
    static quint32 nativeModifiers(Qt::KeyboardModifiers modifiers);

    static bool    registerShortcut(quint32 nativeKey, quint32 nativeMods);
    static bool    unregisterShortcut(quint32 nativeKey, quint32 nativeMods);

    static QHash<QPair<quint32, quint32>, GlobalShortcut *> _shortcuts;
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
