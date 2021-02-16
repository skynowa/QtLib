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
    Q_OBJECT

    void run() override;
    void end();

Q_SIGNALS:
    void sig_activated();

private:
    bool _isTerminate {};
};
//-------------------------------------------------------------------------------------------------
