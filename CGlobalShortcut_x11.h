/**
 * \file   CGlobalShortcut_x11.h
 * \brief  Global shortcut
 */


#ifndef QtLib_CGlobalShortcut_x11H
#define QtLib_CGlobalShortcut_x11H
//-------------------------------------------------------------------------------------------------
#include "Common.h"
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
};
//-------------------------------------------------------------------------------------------------
#endif // QtLib_CGlobalShortcut_x11H
