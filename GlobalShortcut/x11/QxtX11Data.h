/**
 * \file
 * \brief
 */


#pragma once

#include "../Global.h"

#include <X11/Xlib.h>
#include <xcb/xcb.h>
#include "QxtX11ErrorHandler.h"
//-------------------------------------------------------------------------------------------------
namespace qtlib
{

class QxtX11Data
{
public:
    QxtX11Data();
   ~QxtX11Data();

    KeyCode  keysymToKeycode(KeySym keysym);

    bool     grabKey(quint32 keycode, quint32 modifiers);
    bool     ungrabKey(quint32 keycode, quint32 modifiers);

private:
    Display *_display {};
    Window   _rootWindow {};

    QxtX11ErrorHandler _errorHandler;
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
