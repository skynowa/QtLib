/**
 * \file  ShortcutActivator.h
 * \brief
 *
 * xgrabkey.c - https://gist.github.com/jouyouyun/669726de58df8d333666
 */

#pragma once

#include "../Global.h"
#include <QThread>
//-------------------------------------------------------------------------------------------------
class ShortcutActivator :
    public QThread
{
public:
    ShortcutActivator(void *display, const quint32 keycode, const quint32 modifiers);

    void run() final;
    void end();

Q_SIGNALS:
    void sig_activated(quint32, quint32);

private:
    Q_OBJECT

    void          *_display {};
    const quint32  _keycode {};
    const quint32  _modifiers {};

    bool _isTerminate {};
};
//-------------------------------------------------------------------------------------------------
