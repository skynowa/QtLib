/**
 * \file
 * \brief
 */


#pragma once

#include "../Global.h"

#include <type_traits>
#include <X11/Xlib.h>
#include <xcb/xcb.h>
//--------------------------------------------------------------------------------------------------
namespace qtlib
{

class QxtX11ErrorHandler
{
public:
    static Bool isError;

    QxtX11ErrorHandler();
   ~QxtX11ErrorHandler();

    void set();

private:
    using x11_error_handler_t = std::add_pointer<Bool(Display *display, XErrorEvent *event)>::type;
    x11_error_handler_t _errorHandlerLast {};

    static
    int  qxtX11ErrorHandler(Display *display, XErrorEvent *event);

    static
    void errorText(Display *display, XErrorEvent *event);
};

} // namespace qtlib
//--------------------------------------------------------------------------------------------------
