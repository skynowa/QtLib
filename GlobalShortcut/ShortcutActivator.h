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
    quint32  keycode {};
    quint32  modifiers {};

    void run() override;
    void end();

Q_SIGNALS:
    void sig_activated(quint32, quint32);

private:
    bool _isTerminate {};

    Q_OBJECT
};
//-------------------------------------------------------------------------------------------------
