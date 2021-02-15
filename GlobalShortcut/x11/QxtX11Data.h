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

//-------------------------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------------------------
class QxtX11Data
{
public:
    QxtX11Data();

    bool     isValid();
    Display *display();
    Window   rootWindow();

    bool grabKey(quint32 keycode, quint32 modifiers, Window window);
    bool ungrabKey(quint32 keycode, quint32 modifiers, Window window);

private:
    Display *_display {};
};
//-------------------------------------------------------------------------------------------------

} // namespace qtlib
