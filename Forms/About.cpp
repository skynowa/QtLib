/**
 * \file   About.cpp
 * \brief  about widget
 */


#include "About.h"

#include <QtLib/Common.h>
#include <QtLib/Utils.h>
#include <QtLib/Application.h>

#include <xLib/xLib.h>
#include <xLib/Core/Application.h>


/**************************************************************************************************
*   public
*
**************************************************************************************************/

xLIB_CORE_APPLICATION_STATIC_DECLARE

//-------------------------------------------------------------------------------------------------
About::About(
    QWidget *a_parent
) :
    QDialog(a_parent)
{
    _construct();
}
//-------------------------------------------------------------------------------------------------


/**************************************************************************************************
*   private
*
**************************************************************************************************/

//-------------------------------------------------------------------------------------------------
void
About::_construct()
{
    _initMain();
}
//-------------------------------------------------------------------------------------------------
void
About::_destruct()
{
}
//-------------------------------------------------------------------------------------------------
void
About::_initMain()
{
    ui.setupUi(this);

    // title
    {
        cQString title = QString(tr("<p><b>%1</b></p>"))
                            .arg( qS2QS(xlib::core::Application::name()) )
                            .arg( qS2QS(xlib::core::Application::versionFull()) );

        ui.lblAppName->setText(title);
    }

    // tab "About"
    {
        xlib::debug::BuildInfo info;

        cQString painText = QString(
            tr(
            "%1\n\n"
            "Build info:\n"
            "Type: %2\n"
            "Date time: %3\n"
            "Language standart: %4\n"
            "OS environment: %5\n"
            "OS: %6\n"
            "Architecture: %7\n"
            "Compiler: %8\n"
            "Character encoding: %9\n"
            "LibC: %10\n"
            "LibC++: %11\n"
            "xLib: %12\n"
            "Qt: %13\n"
            "Binary type: %14\n")
            .arg( xlib::core::Application::decription().c_str() )
            .arg( info.isRelease() ? "Release" : "Debug" )
            .arg( info.datetime().c_str() )
            .arg( info.langStandart().c_str() )
            .arg( info.osEnvironment().c_str() )
            .arg( info.os().c_str() )
            .arg( info.arch().c_str() )
            .arg( info.compiler().c_str() )
            .arg( info.isUnicodeEncoding() ? "Unicode" : "Ansi" )
            .arg( info.stdLibC().c_str() )
            .arg( info.stdLibCpp().c_str() )
            .arg( info.xlibVersion().c_str() )
            .arg( info.qt().c_str() )
            .arg( info.binaryType().c_str() ));

        ui.pteAbout->appendPlainText(painText);
    }

    //  signals, slots
    {
        connect(ui.bbxButtons, SIGNAL( clicked() ),
                this,          SLOT  ( slot_bbxButtons_onClicked() ));
    }
}
//-------------------------------------------------------------------------------------------------
void
About::slot_bbxButtons_onClicked(
    QAbstractButton *a_button
)
{
    QDialogButtonBox::StandardButton sbRv = ui.bbxButtons->standardButton(a_button);
    switch (sbRv) {
    case QDialogButtonBox::Ok:
    default:
        close();
        break;
    }
}
//-------------------------------------------------------------------------------------------------
