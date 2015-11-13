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
struct AboutData
{
    QString appName;
    QString appVersionFull;
    QString appDecription;
    QString appCopyrightYears;
    QString appUsage;
    QString appHelp;
    QString appVersionMajor;
    QString appVersionMinor;
    QString appVersionPatch;
    QString appVersionType;
    QString appVersionRevision;
    QString appVendorName;
    QString appVendorDomain;
    QString appVendorAuthor;
    QString appVendorUrl;
    QString appVendorEmail;
    QString appVendorSkype;

    QString appDonatePayPal;
    QString appDonateWebMoney;
    QString appDonateYandexMoney;
    QString appDonatePrivate24;
};

typedef const AboutData cAboutData;
//-------------------------------------------------------------------------------------------------
class About :
    public QDialog
{
public:
    Ui::UiAbout ui;

    explicit    About(QWidget *parent, cAboutData &data, const QIcon &icon);

private:
    cAboutData  &_data;
    const QIcon &_icon;

    void        _construct();
    void        _destruct();
    void        _constructUi();

    Q_OBJECT
    Q_DISABLE_COPY(About)

private Q_SLOTS:
    void        bbxButtons_onClicked(QAbstractButton *button);
};
//-------------------------------------------------------------------------------------------------
