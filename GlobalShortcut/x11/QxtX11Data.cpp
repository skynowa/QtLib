/**
 * \file
 * \brief
 *
 * http://incise.org/xlib-key-passing.html
 * https://github.com/kupferlauncher/keybinder
 */


#include "QxtX11Data.h"

#include <QProcessEnvironment>
#include "QxtX11ErrorHandler.h"

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

    _display = ::XOpenDisplay(displayName);
    /// _display = ::XOpenDisplay(NULL);
	if (_display == nullptr) {
		qDebug() << "XOpenDisplay: " << ::XDisplayName(displayName);
	}
#endif

    _rootWindow = DefaultRootWindow(_display);
}
//--------------------------------------------------------------------------------------------------
bool
QxtX11Data::isValid()
{
	return (_display != Q_NULLPTR);
}
//--------------------------------------------------------------------------------------------------
KeyCode
QxtX11Data::keysymToKeycode(
    KeySym a_keysym
)
{
    const KeyCode keyCode = ::XKeysymToKeycode(_display, a_keysym);
    qTEST(keyCode != 0);

    return keyCode;
}
//--------------------------------------------------------------------------------------------------
/**
 * http://incise.org/xlib-key-passing.html
 */
bool
QxtX11Data::grabKey(
	quint32 a_keycode,
    quint32 a_modifiers
)
{
	QxtX11ErrorHandler errorHandler;
	qDebug() << "grabKey 1: " << qTRACE_VAR(errorHandler.isError);

	for (int i = 0; !errorHandler.isError && i < maskModifiers.size(); ++ i) {
        int iRv = ::XGrabKey(_display, a_keycode, a_modifiers | maskModifiers[i], _rootWindow, True,
			GrabModeAsync, GrabModeAsync);
		// qTEST(iRv == 0);
		// if (iRv != 0) {
			qDebug() << "XGrabKey: " << qTRACE_VAR(iRv);
		// }
	}

	qDebug() << "grabKey 2: " << qTRACE_VAR(errorHandler.isError);
	if (errorHandler.isError) {
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
	QxtX11ErrorHandler errorHandler;

	for (const auto &maskMods : maskModifiers) {
        int iRv = ::XUngrabKey(_display, a_keycode, a_modifiers | maskMods, _rootWindow);
		if (iRv != 0) {
			qDebug() << "XUngrabKey: " << qTRACE_VAR(iRv);
		}
		/// qTEST(iRv == 0);
	}

	return !errorHandler.isError;
}
//-------------------------------------------------------------------------------------------------

} // namespace qtlib
