/**
 * \file
 * \brief
 */


#pragma once

#include "../Global.h"

#include <X11/Xlib.h>
#include <xcb/xcb.h>
//-------------------------------------------------------------------------------------------------
namespace qtlib
{

class QxtX11Data
{
public:
    QxtX11Data();

    bool     isValid();

    KeyCode  keysymToKeycode(KeySym keysym);

    bool     grabKey(quint32 keycode, quint32 modifiers);
    bool     ungrabKey(quint32 keycode, quint32 modifiers);

private:
    Display *_display {};
    Window   _rootWindow {};
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
