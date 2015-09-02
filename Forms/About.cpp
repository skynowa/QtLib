/**
 * \file   About.cpp
 * \brief  about widget
 */


#include "About.h"


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
    QDialog              (a_parent),
    _appDonatePayPal     (),
    _appDonateWebMoney   (),
    _appDonateYandexMoney(),
    _appDonatePrivate24  (),
    _icon                (a_icon)
{
    _construct();
}
//-------------------------------------------------------------------------------------------------
void
About::setAppDonatePayPal(
    cQString &a_value
)
{
    _appDonatePayPal = a_value;
}
//-------------------------------------------------------------------------------------------------
void
About::setAppDonateWebMoney(
    cQString &a_value
)
{
    _appDonateWebMoney = a_value;
}
//-------------------------------------------------------------------------------------------------
void
About::setAppDonateYandexMoney(
    cQString &a_value
)
{
    _appDonateYandexMoney = a_value;
}
//-------------------------------------------------------------------------------------------------
void
About::setAppDonatePrivate24(
    cQString &a_value
)
{
    _appDonatePrivate24 = a_value;
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
        ui.lblAppIcon->setPixmap( windowIcon().pixmap(32) );

        cQString title = QString(tr("<p><b>%1 %2</b></p>"))
            .arg( xlib::core::Application::name().c_str() )
            .arg( xlib::core::Application::versionFull().c_str() );

        ui.lblAppName->setText(title);
    }

    // tab "About"
    {
        cQString text = QString(tr(
            "<b>About</b><br>"
            "<br>"
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

        ui.txtbAbout->setHtml(text);
    }

    // tab "Authors"
    {
        cQString text = QString(tr(
            "<b>Authors</b><br>"
            "<br>"
            "Vendor: %1<br>"
            "Domain: %2<br>"
            "Authors: %3<br>"
            "E-mail: %4 (<a href=\"mailto:%4?subject=%5\">send</a>)<br>"
            "Skype: %6 (<a href=\"skype:%6?add\">add</a>, "
                       "<a href=\"skype:%6?chat\">chat</a>, "
                       "<a href=\"skype:%6?call\">call</a>)<br>"
            "Jabber: %7<br>"
            "ICQ: %8<br>"
            "Web: %9 (<a href=\"%9\">go to browser</a>)<br>"))
            .arg( xlib::core::Application::vendorName().c_str() )
            .arg( xlib::core::Application::vendorDomain().c_str() )
            .arg( xlib::core::Application::vendorAuthor().c_str() )
            .arg( xlib::core::Application::vendorEmail().c_str() )
            .arg( xlib::core::Application::name().c_str() )
            .arg( xlib::core::Application::vendorSkype().c_str() )
            .arg("skynowa@jabber.ru")
            .arg("627713628")
            .arg( xlib::core::Application::vendorUrl().c_str() );

        ui.txtbAuthors->setHtml(text);
    }

    // tab "Thanks to"
    {
        cQString text =
            "<b>Thanks to</b><br>";

        ui.txtbThanks->setHtml(text);
    }

    // tab "License"
    {
        cQString text =
            "<b>License</b>"
            "<pre>"
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
            "along with this program.  If not, see <http://www.gnu.org/licenses/>.<br>"
            "</pre>";

        ui.txtbLicense->setHtml(text);
    }

    // tab "Report Bugs"
    {
        cQString text = QString(tr(
            "<b>Report bugs or wishes</b><br>"
            "<br>"
            "E-mail: %1 (<a href=\"mailto:%1?subject=%2\">send</a>)<br>"
            "Skype: %3 (<a href=\"skype:%3?add\">add</a>, "
                       "<a href=\"skype:%3?chat\">chat</a>, "
                       "<a href=\"skype:%3?call\">call</a>)<br>"
            "Jabber: %4<br>"
            "ICQ: %5<br>"
            "Web: %6 (<a href=\"%6\">go to browser</a>)<br>"))
            .arg( xlib::core::Application::vendorEmail().c_str() )
            .arg( xlib::core::Application::name().c_str() )
            .arg( xlib::core::Application::vendorSkype().c_str() )
            .arg("skynowa@jabber.ru")
            .arg("627713628")
            .arg( xlib::core::Application::vendorUrl().c_str() );

        ui.txtbReportBugs->setHtml(text);
    }

    // tab "Build info"
    {
        const xlib::debug::BuildInfo &info = xlib::core::Application::buildInfo();

        cQString text = QString(tr(
            "<b>Build info</b><br>"
            "<br>"
            "Type: %1<br>"
            "Date time: %2<br>"
            "Language standart: %3<br>"
            "OS environment: %4<br>"
            "OS: %5<br>"
            "Architecture: %6<br>"
            "Architecture bits: %7<br>"
            "Compiler: %8<br>"
            "Character encoding: %9<br>"
            "LibC: %10<br>"
            "LibC++: %11<br>"
            "Qt: %12<br>"
            "xLib: %13<br>"
            "Binary type: %14<br>"))
            .arg( info.isRelease() ? "Release" : "Debug" )
            .arg( info.datetime().c_str() )
            .arg( info.langStandart().c_str() )
            .arg( info.osEnvironment().c_str() )
            .arg( info.os().c_str() )
            .arg( info.arch().c_str() )
            .arg( info.bitsArch().c_str() )
            .arg( info.compiler().c_str() )
            .arg( info.isUnicodeEncoding() ? "Unicode" : "Ansi" )
            .arg( info.stdLibC().c_str() )
            .arg( info.stdLibCpp().c_str() )
            .arg( info.qt().c_str() )
            .arg( info.xlibVersion().c_str() )
            .arg( info.binaryType().c_str() );

        ui.txtbBuildinfo->setHtml(text);
    }

    // tab "Donate"
    {
        cQString text = QString(tr(
            "<b>Donate</b><br>"
            "<br>"
            "PayPal -  %1 (<a href=\"https://www.paypal.com/\">donate</a>)<br>"
            "WebMoney - %2 (<a href=\"http://www.webmoney.ru/\">donate</a>)<br>"
            "YandexMoney - %3 (<a href=\"https://money.yandex.ru/\">donate</a>)<br>"
            "Private24 - %4 (<a href=\"https://privat24.ua/\">donate</a>)<br>"))
            .arg(_appDonatePayPal)
            .arg(_appDonateWebMoney)
            .arg(_appDonateYandexMoney)
            .arg(_appDonatePrivate24);

        ui.txtbDonate->setHtml(text);
    }

    //  signals, slots
    {
        connect(ui.bbxButtons, SIGNAL( clicked(QAbstractButton *) ),
                this,          SLOT  ( bbxButtons_onClicked(QAbstractButton *) ));
    }
}
//-------------------------------------------------------------------------------------------------
void
About::bbxButtons_onClicked(
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
