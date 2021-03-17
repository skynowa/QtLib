/**
 * \file  X11Data.cpp
 * \brief
 */


#include "X11Data.h"

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
X11Data::X11Data()
{
    // could be the value of $DISPLAY or nullptr
    cQString &displayName = QProcessEnvironment::systemEnvironment().value("DISPLAY", ":0.0");

    /// _display = ::XOpenDisplay(displayName.toStdString().c_str());
    _display = ::XOpenDisplay(nullptr);
	if (_display == nullptr) {
        qDebug() << "XOpenDisplay: " << ::XDisplayName(displayName.toStdString().c_str());
	}

    _errorHandler.set();

    _rootWindow = DefaultRootWindow(_display);
}
//--------------------------------------------------------------------------------------------------
X11Data::~X11Data()
{
    if (_display != nullptr) {
        ::XCloseDisplay(_display);
        _display = nullptr;
    }
}
//--------------------------------------------------------------------------------------------------
KeyCode
X11Data::keysymToKeycode(
    const KeySym a_keysym
)
{
    const KeyCode keyCode = ::XKeysymToKeycode(_display, a_keysym);
    qTEST(keyCode != 0);

    return keyCode;
}
//--------------------------------------------------------------------------------------------------
bool
X11Data::grabKey(
    const quint32 a_keycode,
    const quint32 a_modifiers
)
{
    qTEST(a_keycode > 0);
    /// qTEST(a_modifiers > 0);

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

    if (_errorHandler.isError) {
        bool bRv = ungrabKey(a_keycode, a_modifiers);
		qTEST(bRv);

		return false;
	}

	return true;
}
//--------------------------------------------------------------------------------------------------
bool
X11Data::ungrabKey(
    const quint32 a_keycode,
    const quint32 a_modifiers
)
{
    qTEST(a_keycode > 0);
    /// qTEST(a_modifiers > 0);

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
