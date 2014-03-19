/**
 * \file   QxGlobalShortcut_x11.h
 * \brief  Global shortcut
 */


#ifndef QtLib_QxGlobalShortcut_x11H
#define QtLib_QxGlobalShortcut_x11H
//-------------------------------------------------------------------------------------------------
#include "Common.h"
//-------------------------------------------------------------------------------------------------
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
//-------------------------------------------------------------------------------------------------
class QxGlobalShortcut_x11
    : QMainWindow
{
public:
                 QxGlobalShortcut_x11(QWidget *parent);
    virtual     ~QxGlobalShortcut_x11();

    virtual bool eventFilter(QObject *object, QEvent *event);

private:
    int          _keyCode;
    Display     *_display;
};
//-------------------------------------------------------------------------------------------------
#endif // QtLib_QxGlobalShortcut_x11H
