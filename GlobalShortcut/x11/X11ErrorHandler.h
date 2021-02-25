/**
 * \file
 * \brief
 */


#pragma once

#include "../Global.h"

#include <X11/Xlib.h>
#include <xcb/xcb.h>
//--------------------------------------------------------------------------------------------------
namespace qtlib
{

class X11ErrorHandler
{
public:
    X11ErrorHandler();
   ~X11ErrorHandler();

    void set();

    static Bool isError;

private:
    XErrorHandler _errorHandlerLast {};

    static
    int  _OnError(Display *display, XErrorEvent *event);

    static
    void _errorText(Display *display, XErrorEvent *event);
};

} // namespace qtlib
//--------------------------------------------------------------------------------------------------
