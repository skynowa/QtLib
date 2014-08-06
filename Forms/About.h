/**
 * \file   About.h
 * \brief  about widget
 */


#ifndef QtLib_AboutH
#define QtLib_AboutH
//-------------------------------------------------------------------------------------------------
#include "ui_About.h"

#include <QtLib/Common.h>
#include <QtLib/Utils.h>
#include <QtLib/Application.h>

#include <xLib/xLib.h>
#include <xLib/Core/Application.h>
//-------------------------------------------------------------------------------------------------
class About :
    public QDialog
{
    Q_OBJECT

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
    void        _initMain();

private Q_SLOTS:
    void        slot_bbxButtons_onClicked(QAbstractButton *button);
};
//-------------------------------------------------------------------------------------------------
#endif // QtLib_AboutH
