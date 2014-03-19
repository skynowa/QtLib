/**
 * \file   CGlobalShortcut_x11.h
 * \brief  Global shortcut
 */


#ifndef QtLib_CGlobalShortcut_x11H
#define QtLib_CGlobalShortcut_x11H
//-------------------------------------------------------------------------------------------------
#include "Common.h"
//-------------------------------------------------------------------------------------------------
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
//-------------------------------------------------------------------------------------------------
class CGlobalShortcut_x11
    : QMainWindow
{
public:
                 CGlobalShortcut_x11(QWidget *parent);
    virtual     ~CGlobalShortcut_x11();

    virtual bool eventFilter(QObject *object, QEvent *event);

private:
    int          _keyCode;
    Display     *_display;
};
//-------------------------------------------------------------------------------------------------
#endif // QtLib_CGlobalShortcut_x11H
