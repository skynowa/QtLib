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
    void    *display {};
    int      keycode {};
    quint32  modifiers {};

    void run() override;
    void end();

Q_SIGNALS:
    void sig_activated(int, quint32);

private:
    bool _isTerminate {};

    Q_OBJECT
};
//-------------------------------------------------------------------------------------------------
