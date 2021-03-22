/**
 * \file  GlobalShortcut.h
 * \brief The GlobalShortcut class provides a global shortcut aka "hotkey".
 *
 * A global shortcut triggers even if the application is not active. This
 * makes it easy to implement applications that react to certain shortcuts
 * still if some other application is active or if the application is for
 * example minimized to the system tray.
 *
 * Example usage:
 * \code
 * GlobalShortcut* shortcut = new GlobalShortcut(window);
 * connect(shortcut, SIGNAL(activated()), window, SLOT(toggleVisibility()));
 * shortcut->setShortcut(QKeySequence("Ctrl+Shift+F12"));
 * \endcode
 */


#pragma once

#include <QObject>
#include <QKeySequence>

#include "Global.h"
//-------------------------------------------------------------------------------------------------
namespace qtlib
{

class GlobalShortcut_impl;

class GlobalShortcut :
    public QObject
{
public:
    explicit     GlobalShortcut(QObject *parent);
    explicit     GlobalShortcut(const QKeySequence &shortcut, QObject *parent);
    virtual     ~GlobalShortcut();

    QKeySequence get() const;
    bool         set(const QKeySequence &shortcut);

    bool         isEnabled() const;

public Q_SLOTS:
    void         setEnabled(bool enabled = true);
    void         setDisabled(bool disabled = true);

Q_SIGNALS:
    void         sig_activated();
        ///< This signal is emitted when the user types the shortcut's key sequence

private:
    Q_OBJECT
    QTLIB_DECLARE_PRIVATE(GlobalShortcut)

    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)
    Q_PROPERTY(QKeySequence shortcut READ get WRITE set)
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
