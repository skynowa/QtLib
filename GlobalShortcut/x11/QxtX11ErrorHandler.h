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

class QxtX11ErrorHandler
{
public:
    QxtX11ErrorHandler();
   ~QxtX11ErrorHandler();

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
