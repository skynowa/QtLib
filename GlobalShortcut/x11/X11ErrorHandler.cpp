/**
 * \file
 * \brief
 */


#include "X11ErrorHandler.h"

namespace qtlib
{

//-------------------------------------------------------------------------------------------------
Bool X11ErrorHandler::isError {};
//-------------------------------------------------------------------------------------------------
X11ErrorHandler::X11ErrorHandler()
{
	isError = False;
}
//-------------------------------------------------------------------------------------------------
X11ErrorHandler::~X11ErrorHandler()
{
    XErrorHandler errorHandlerLast = ::XSetErrorHandler(_errorHandlerLast);
	qTEST_PTR(errorHandlerLast);
}
//-------------------------------------------------------------------------------------------------
void
X11ErrorHandler::set()
{
    // qTRACE_FUNC;

    isError = False;

    _errorHandlerLast = ::XSetErrorHandler(_OnError);
    qTEST_PTR(_errorHandlerLast);
}
//-------------------------------------------------------------------------------------------------
/* static  */
int
X11ErrorHandler::_OnError(
	Display     *a_display,
	XErrorEvent *a_event
)
{
    // qTRACE_FUNC;

	qTEST_PTR(a_display);
	qTEST_PTR(a_event);

	if (a_event->error_code == Success) {
		isError = False;
		qDebug() << "Success";
		return 0;
	}

	isError = True;
    _errorText(a_display, a_event);

	switch (a_event->error_code) {
	case BadAccess:
	case BadValue:
	case BadWindow:
	// case BadMatch:
		if (a_event->request_code == 33 /* X_GrabKey */ ||
			a_event->request_code == 34 /* X_UngrabKey */)
		{
			///
		}
	}

	return 1;
}
//-------------------------------------------------------------------------------------------------
/* static */
void
X11ErrorHandler::_errorText(
	Display     *a_display,
	XErrorEvent *a_event
)
{
    // could be the value of $DISPLAY or nullptr
    cQString &displayName = QProcessEnvironment::systemEnvironment().value("DISPLAY", ":0.0");

	char errorText[255 + 1] {};
	::XGetErrorText(a_display, a_event->error_code, errorText, sizeof(errorText) - 1);

#if 1
    qDebug()
        << "XErrorEvent:" << "\n"
        << "Type:    " << a_event->type << "\n"
        << "Dispaly: " << ::XDisplayName(displayName.toStdString().c_str()) << "\n"
        << "Resource:" << a_event->resourceid << "\n"
        << "Serial:  " << a_event->serial << "\n"
        << "Pp-code: " << a_event->request_code << "/" << a_event->minor_code << "\n"
        << "Error:   " << a_event->error_code << "-" << errorText << "\n";
#endif
}
//-------------------------------------------------------------------------------------------------

} // namespace qtlib
