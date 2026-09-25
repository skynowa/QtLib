/**
 * \file   About.h
 * \brief  about widget
 */


#pragma once

#include "ui_About.h"

#include "../Common.h"
#include "../Utils.h"
#include "../Application.h"

//-------------------------------------------------------------------------------------------------
struct AboutData
{
    QString appName;
    QString appDecription;
    QString appCopyrightYears;
    QString appUsage;
    QString appHelp;

    QString appVersionFull;
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

    QString appDonatePayPal;
    QString appDonateWebMoney;
    QString appDonateYandexMoney;
    QString appDonatePrivate24;
};

qUSING_CONST(AboutData);
//-------------------------------------------------------------------------------------------------
class About :
    public QDialog
{
public:
    About() = delete;
    About(QWidget *parent, cAboutData &data, cQIcon &icon);

private:
    Ui::UiAbout ui;

    cAboutData &_data;
    cQIcon     &_icon;

    void _construct();
    void _destruct();
    void _constructUi();

    Q_OBJECT
    Q_DISABLE_COPY(About)

private Q_SLOTS:
    void        bbxButtons_onClicked(QAbstractButton *button);
};
//-------------------------------------------------------------------------------------------------
