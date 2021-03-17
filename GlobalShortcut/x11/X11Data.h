/**
 * \file  X11Data.h
 * \brief
 *
 * http://incise.org/xlib-key-passing.html
 * https://github.com/kupferlauncher/keybinder
 */



#pragma once

#include "../Global.h"

#include <X11/Xlib.h>
#include <xcb/xcb.h>
#include "X11ErrorHandler.h"
//-------------------------------------------------------------------------------------------------
namespace qtlib
{

class X11Data
{
public:
    X11Data();
   ~X11Data();

    KeyCode keysymToKeycode(const KeySym keysym);

    bool    grabKey(const quint32 keycode, const quint32 modifiers);
    bool    ungrabKey(const quint32 keycode, const quint32 modifiers);

private:
    static const QVector<quint32> _modifiers;

    Display        *_display {};
    Window          _rootWindow {};
    X11ErrorHandler _errorHandler;
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
