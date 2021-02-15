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
    Display *display();
    Window   rootWindow();

    bool     grabKey(quint32 keycode, quint32 modifiers, Window window);
    bool     ungrabKey(quint32 keycode, quint32 modifiers, Window window);

private:
    Display *_display {};
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
