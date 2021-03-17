/**
 * \file  ShortcutActivator.h
 * \brief
 */

#pragma once

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
