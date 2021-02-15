/**
 * \file
 * \brief
 */


#pragma once

// Qt 5.11.1: don't supprt Qt Platform Abstraction (QPA)
#define QT_QPA 0

#if QT_QPA
#include <qpa/qplatformnativeinterface.h>
#endif

#include <X11/Xlib.h>
#include <xcb/xcb.h>
#include <QProcessEnvironment>

namespace qtlib
{

//--------------------------------------------------------------------------------------------------
class QxtX11ErrorHandler
{
public:
    static Bool isError;

    QxtX11ErrorHandler();
   ~QxtX11ErrorHandler();

private:
    using x11_error_handler_t = std::add_pointer<Bool(Display *display, XErrorEvent *event)>::type;
    x11_error_handler_t _errorHandlerLast {};

    static
    int qxtX11ErrorHandler(Display *display, XErrorEvent *event);

    static
    void errorText(Display *display, XErrorEvent *event);
};
//--------------------------------------------------------------------------------------------------

} // namespace qtlib
