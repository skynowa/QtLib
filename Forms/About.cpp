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
    QWidget     *a_parent,
    const QIcon &a_icon
) :
    QDialog(a_parent),
    _icon  (a_icon)
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

    // dialog title
    {
        cQString dialogTitle = QString(tr("%1 - about"))
            .arg( xlib::core::Application::name().c_str() );

        setWindowTitle(dialogTitle);
        setWindowIcon(_icon);
    }

    // title
    {
        cQString title = QString(tr("<p><b>%1 %2</b></p>"))
            .arg( xlib::core::Application::name().c_str() )
            .arg( xlib::core::Application::versionFull().c_str() );

        ui.lblAppName->setText(title);
    }

    // tab "About"
    {
        cQString text = QString(tr(
            "%1\n"
            "Copyright %2 %3. All rights reserved.\n"
            "\n"
            "Help:"
            "%4\n"
            "\n"
            "Usage:"
            "%5\n"))
            .arg( xlib::core::Application::decription().c_str() )
            .arg( xlib::core::Application::copyrightYears().c_str() )
            .arg( xlib::core::Application::vendorName().c_str() )
            .arg( xlib::core::Application::help().c_str() )
            .arg( xlib::core::Application::usage().c_str() );

        ui.pteAbout->setHtml(text);
    }

    // tab "Authors"
    {
        cQString text = QString(tr(
            "Vendor: %1\n"
            "Domain: %2\n"
            "Authors: %3\n"
            "E-mail: %4\n"
            "Skype: %5\n"
            "Jabber: %6\n"
            "ICQ: %7\n"
            "Web: %8\n"))
            .arg( xlib::core::Application::vendorName().c_str() )
            .arg( xlib::core::Application::vendorDomain().c_str() )
            .arg( xlib::core::Application::vendorAuthor().c_str() )
            .arg( xlib::core::Application::vendorEmail().c_str() )
            .arg( xlib::core::Application::vendorSkype().c_str() )
            .arg("skynowa@jabber.ru")
            .arg("627713628")
            .arg( xlib::core::Application::vendorUrl().c_str() );

        ui.pteAuthors->setHtml(text);
    }

    // tab "Thanks to"
    {
        cQString text;

        ui.pteThanks->setHtml(text);
    }

    // tab "License"
    {
        cQString text =
            "    This program is free software: you can redistribute it and/or modify\n"
            "it under the terms of the GNU General Public License as published by\n"
            "the Free Software Foundation, either version 3 of the License, or\n"
            "(at your option) any later version.\n"
            "\n"
            "    This program is distributed in the hope that it will be useful,\n"
            "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
            "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
            "GNU General Public License for more details.\n"
            "\n"
            "    You should have received a copy of the GNU General Public License\n"
            "along with this program.  If not, see <http://www.gnu.org/licenses/>.\n";

        ui.pteLicense->setHtml(text);
    }

    // tab "Report Bugs"
    {
        cQString text = QString(tr(
            "Report bugs or wishes.\n"
            "\n"
            "E-mail: %1\n"
            "Skype: %2\n"
            "Jabber: %3\n"
            "ICQ: %4\n"
            "Web: %5\n"))
            .arg( xlib::core::Application::vendorEmail().c_str() )
            .arg( xlib::core::Application::vendorSkype().c_str() )
            .arg("skynowa@jabber.ru")
            .arg("627713628")
            .arg( xlib::core::Application::vendorUrl().c_str() );

        ui.pteReportBugs->setText(text);
    }

    // tab "Build info"
    {
        xlib::debug::BuildInfo info;

        cQString text = QString(tr(
            "Type: %1\n"
            "Date time: %2\n"
            "Language standart: %3\n"
            "OS environment: %4\n"
            "OS: %5\n"
            "Architecture: %6\n"
            "Compiler: %7\n"
            "Character encoding: %8\n"
            "LibC: %9\n"
            "LibC++: %10\n"
            "xLib: %11\n"
            "Qt: %12\n"
            "Binary type: %13\n"))
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
            .arg( info.binaryType().c_str() );

        ui.pteBuildinfo->setHtml(text);
    }

    // tab "Donate"
    {
        cQString text = QString(tr(
            "PayPal -  %1\n"
            "WebMoney - %2\n"
            "YandexMoney - %3\n"
            "Private24 - %4\n"))
            .arg("[TODO]")
            .arg("[TODO]")
            .arg("[TODO]")
            .arg("[TODO]");

        ui.pteDonate->setHtml(text);
    }

    //  signals, slots
    {
        connect(ui.bbxButtons, SIGNAL( clicked(QAbstractButton *) ),
                this,          SLOT  ( slot_bbxButtons_onClicked(QAbstractButton *) ));
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
