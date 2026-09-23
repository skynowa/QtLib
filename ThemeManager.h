/**
 * \file  ThemeManager.h
 * \brief Declares application theme management.
 */


#pragma once

#include <QString>

class QApplication;
class QPalette;
//-------------------------------------------------------------------------------------------------
class ThemeManager final
{
public:
    // Supported application themes
    enum class Theme
    {
        Light,
        LightGray,
        Grey,
        MediumGray,
        DarkGrey,
        Dark,
    };

    // Construction
    explicit ThemeManager(QApplication *application);

    // Theme conversion and application
    Theme   parse(const QString &value) const;
    QString name(Theme theme) const;
    void    apply(Theme theme) const;

private:
    QApplication *_application {};

    QPalette _palette(Theme theme) const;
    QString  _styleSheet(Theme theme) const;
};
//-------------------------------------------------------------------------------------------------
