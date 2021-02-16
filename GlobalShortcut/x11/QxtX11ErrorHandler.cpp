/**
 * \file
 * \brief
 */


#include "QxtX11ErrorHandler.h"

namespace qtlib
{

//-------------------------------------------------------------------------------------------------
Bool QxtX11ErrorHandler::isError {};
//-------------------------------------------------------------------------------------------------
QxtX11ErrorHandler::QxtX11ErrorHandler()
{
	isError = False;
}
//-------------------------------------------------------------------------------------------------
QxtX11ErrorHandler::~QxtX11ErrorHandler()
{
	x11_error_handler_t errorHandlerLast = ::XSetErrorHandler(_errorHandlerLast);
	qTEST_PTR(errorHandlerLast);
}
//-------------------------------------------------------------------------------------------------
void
QxtX11ErrorHandler::set()
{
    qTRACE_FUNC;

    isError = False;

    _errorHandlerLast = ::XSetErrorHandler(qxtX11ErrorHandler);
    qTEST_PTR(_errorHandlerLast);
}
//-------------------------------------------------------------------------------------------------
/* static  */
int
QxtX11ErrorHandler::qxtX11ErrorHandler(
	Display     *a_display,
	XErrorEvent *a_event
)
{
    qTRACE_FUNC;

	qTEST_PTR(a_display);
	qTEST_PTR(a_event);

	if (a_event->error_code == Success) {
		isError = False;
		qDebug() << "Success";
		return 0;
	}

	isError = True;
	errorText(a_display, a_event);

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
QxtX11ErrorHandler::errorText(
	Display     *a_display,
	XErrorEvent *a_event
)
{
	char errorText[255 + 1] {};
	::XGetErrorText(a_display, a_event->error_code, errorText, sizeof(errorText) - 1);

	// could be the value of $DISPLAY or nullptr
	const char *displayName = QProcessEnvironment::systemEnvironment().value("DISPLAY", ":0.0")
									.toStdString().c_str();

	qDebug() << "QxtX11ErrorHandler: "
		<< "Dispaly: " << ::XDisplayName(displayName) << "\n"
		<< "Error:   " << a_event->error_code << " - " << errorText << "\n"
		<< "Event: "   << a_event->type;
}
//-------------------------------------------------------------------------------------------------

} // namespace qtlib
