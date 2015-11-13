/**
 * \file   About.h
 * \brief  about widget
 */


#pragma once

#include "ui_About.h"

#include "../Common.h"
#include "../Utils.h"
#include "../Application.h"

#include <xLib/xLib.h>
#include <xLib/Core/Application.h>
//-------------------------------------------------------------------------------------------------
class About :
    public QDialog
{
public:
    Ui::UiAbout ui;

    explicit    About(QWidget *parent, const QIcon &icon);

    xPROPERTY(QString, appName);
    xPROPERTY(QString, appVersionFull);
    xPROPERTY(QString, appDecription);
    xPROPERTY(QString, appCopyrightYears);
    xPROPERTY(QString, appUsage);
    xPROPERTY(QString, appHelp);
    xPROPERTY(QString, appVersionMajor);
    xPROPERTY(QString, appVersionMinor);
    xPROPERTY(QString, appVersionPatch);
    xPROPERTY(QString, appVersionType);
    xPROPERTY(QString, appVersionRevision);
    xPROPERTY(QString, appVendorName);
    xPROPERTY(QString, appVendorDomain);
    xPROPERTY(QString, appVendorAuthor);
    xPROPERTY(QString, appVendorUrl);
    xPROPERTY(QString, appVendorEmail);
    xPROPERTY(QString, appVendorSkype);

    xPROPERTY(QString, appDonatePayPal);
    xPROPERTY(QString, appDonateWebMoney);
    xPROPERTY(QString, appDonateYandexMoney);
    xPROPERTY(QString, appDonatePrivate24);

public:
    void        construct();

private:
    const QIcon &_icon;

    void        _destruct();
    void        _constructUi();

    Q_OBJECT
    Q_DISABLE_COPY(About)

private Q_SLOTS:
    void        bbxButtons_onClicked(QAbstractButton *button);
};
//-------------------------------------------------------------------------------------------------
