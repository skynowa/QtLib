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
            "%1<br>"
            "Copyright %2 %3. All rights reserved.<br>"
            "<br>"
            "Help:<br>"
            "%4<br>"
            "<br>"
            "Usage:<br>"
            "%5<br>"))
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
            "Vendor: %1<br>"
            "Domain: %2<br>"
            "Authors: %3<br>"
            "E-mail: %4<br>"
            "Skype: %5<br>"
            "Jabber: %6<br>"
            "ICQ: %7<br>"
            "Web: %8<br>"))
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
            "    This program is free software: you can redistribute it and/or modify<br>"
            "it under the terms of the GNU General Public License as published by<br>"
            "the Free Software Foundation, either version 3 of the License, or<br>"
            "(at your option) any later version.<br>"
            "<br>"
            "    This program is distributed in the hope that it will be useful,<br>"
            "but WITHOUT ANY WARRANTY; without even the implied warranty of<br>"
            "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the<br>"
            "GNU General Public License for more details.<br>"
            "<br>"
            "    You should have received a copy of the GNU General Public License<br>"
            "along with this program.  If not, see <http://www.gnu.org/licenses/>.<br>";

        ui.pteLicense->setHtml(text);
    }

    // tab "Report Bugs"
    {
        cQString text = QString(tr(
            "Report bugs or wishes.<br>"
            "<br>"
            "E-mail: %1<br>"
            "Skype: %2<br>"
            "Jabber: %3<br>"
            "ICQ: %4<br>"
            "Web: %5<br>"))
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
            "Type: %1<br>"
            "Date time: %2<br>"
            "Language standart: %3<br>"
            "OS environment: %4<br>"
            "OS: %5<br>"
            "Architecture: %6<br>"
            "Compiler: %7<br>"
            "Character encoding: %8<br>"
            "LibC: %9<br>"
            "LibC++: %10<br>"
            "xLib: %11<br>"
            "Qt: %12<br>"
            "Binary type: %13<br>"))
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
            "PayPal -  %1<br>"
            "WebMoney - %2<br>"
            "YandexMoney - %3<br>"
            "Private24 - %4<br>"))
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
