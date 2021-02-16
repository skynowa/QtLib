/**
 * \file
 * \brief
 *
 * http://incise.org/xlib-key-passing.html
 * https://github.com/kupferlauncher/keybinder
 */


#include "QxtX11Data.h"

#include <QProcessEnvironment>

namespace qtlib
{

//--------------------------------------------------------------------------------------------------
/**
 * maskModifiers
 *
 * Mask        | Value | Key
 * ------------+-------+------------
 * ShiftMask   |     1 | Shift
 * LockMask    |     2 | Caps Lock
 * ControlMask |     4 | Ctrl
 * Mod1Mask    |     8 | Alt
 * Mod2Mask    |    16 | Num Lock
 * Mod3Mask    |    32 | Scroll Lock
 * Mod4Mask    |    64 | Windows
 * Mod5Mask    |   128 | ???
 */
const QVector<quint32> maskModifiers =
    QVector<quint32>() << 0 << Mod2Mask << LockMask << (Mod2Mask | LockMask);
//--------------------------------------------------------------------------------------------------
QxtX11Data::QxtX11Data()
{
#if QT_QPA
	QPlatformNativeInterface *native  = qApp->platformNativeInterface();
	void                     *display = native->nativeResourceForScreen(QByteArray("display"),
			QGuiApplication::primaryScreen());
	_display = reinterpret_cast<Display *>(display);
#else
	// could be the value of $DISPLAY or nullptr
	const char *displayName = QProcessEnvironment::systemEnvironment().value("DISPLAY", ":0.0")
									.toStdString().c_str();

    /// _display = ::XOpenDisplay(displayName);
    _display = ::XOpenDisplay(NULL);
	if (_display == nullptr) {
		qDebug() << "XOpenDisplay: " << ::XDisplayName(displayName);
	}
#endif
    _errorHandler.set();

    _rootWindow = DefaultRootWindow(_display);
}
//--------------------------------------------------------------------------------------------------
QxtX11Data::~QxtX11Data()
{
    ::XCloseDisplay(_display);
}
//--------------------------------------------------------------------------------------------------
KeyCode
QxtX11Data::keysymToKeycode(
    KeySym a_keysym
)
{
    const KeyCode keyCode = ::XKeysymToKeycode(_display, XStringToKeysym("F3") /*a_keysym*/);
    qTEST(keyCode != 0);

    return keyCode;
}
//--------------------------------------------------------------------------------------------------
bool
QxtX11Data::grabKey(
	quint32 a_keycode,
    quint32 a_modifiers
)
{
#if 0
    for (int i = 0; !_errorHandler.isError && i < maskModifiers.size(); ++ i) {
        int iRv = ::XGrabKey(_display, a_keycode, a_modifiers | maskModifiers[i], _rootWindow, True,
			GrabModeAsync, GrabModeAsync);
		// qTEST(iRv == 0);
		// if (iRv != 0) {
            qDebug()
                << "XGrabKey: " << qTRACE_VAR(iRv)
                << qTRACE_VAR(_errorHandler.isError);
		// }
	}
#else
    KeyCode F = XKeysymToKeycode(_display, XStringToKeysym("F3"));

    int iRv = XGrabKey(_display, F /* AnyKey */, AnyModifier /* mod */, _rootWindow, True,
        GrabModeAsync, GrabModeAsync);
    // qTEST(iRv == 0);
    // if (iRv != 0) {
        qDebug()
            << "XGrabKey: " << qTRACE_VAR(iRv)
            << qTRACE_VAR(_errorHandler.isError);
    // }
#endif

    if (_errorHandler.isError) {
        bool bRv = ungrabKey(a_keycode, a_modifiers);
		qTEST(bRv);

		return false;
	}

	return true;
}
//--------------------------------------------------------------------------------------------------
bool
QxtX11Data::ungrabKey(
	quint32 a_keycode,
    quint32 a_modifiers
)
{
	for (const auto &maskMods : maskModifiers) {
        int iRv = ::XUngrabKey(_display, a_keycode, a_modifiers | maskMods, _rootWindow);
		if (iRv != 0) {
            qDebug()
                << "XUngrabKey: " << qTRACE_VAR(iRv)
                << qTRACE_VAR(_errorHandler.isError);
		}
		/// qTEST(iRv == 0);
	}

    return !_errorHandler.isError;
}
//-------------------------------------------------------------------------------------------------

} // namespace qtlib
