/**
 * \file   About.h
 * \brief  about widget
 */


#pragma once

#include "ui_About.h"

#include "../Common.h"
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
};

qUSING_CONST(AboutData);
//-------------------------------------------------------------------------------------------------
class About :
    public QDialog
{
public:
    About() = delete;
    About(QWidget *parent, cAboutData &data, cQIcon &icon);

    Q_DISABLE_COPY_MOVE(About)

private:
    Ui::UiAbout ui;

    cAboutData &_data;
    cQIcon     &_icon;

    void _construct();
    void _destruct();
    void _constructUi();

    Q_OBJECT

private Q_SLOTS:
    void        bbxButtons_onClicked(QAbstractButton *button);
};
//-------------------------------------------------------------------------------------------------
