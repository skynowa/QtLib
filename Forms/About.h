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

    void        setAppDonatePayPal(cQString &value);
    void        setAppDonateWebMoney(cQString &value);
    void        setAppDonateYandexMoney(cQString &value);
    void        setAppDonatePrivate24(cQString &value);

private:
    QString     _appDonatePayPal;
    QString     _appDonateWebMoney;
    QString     _appDonateYandexMoney;
    QString     _appDonatePrivate24;

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
