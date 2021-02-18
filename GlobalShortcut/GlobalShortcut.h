/**
 * \file
 * \brief
 */


#pragma once

#include <QObject>
#include <QKeySequence>

#include "Global.h"
//-------------------------------------------------------------------------------------------------
/**
 * 0 - QThread + XNextEvent
 * 1 - QAbstractEventDispatcher (dont't work on Qt 5.11.1)
 */
#define QTLIB_GLOBAL_SHORTCUT_V1 0
//-------------------------------------------------------------------------------------------------
namespace qtlib
{

class GlobalShortcut_impl;

class GlobalShortcut :
    public QObject
{
public:
    explicit     GlobalShortcut(QObject *parent /* = nullptr */);
    explicit     GlobalShortcut(const QKeySequence &shortcut, QObject *parent /* = nullptr */);
    virtual     ~GlobalShortcut();

    QKeySequence get() const;
    bool         set(const QKeySequence &shortcut);

    bool         isEnabled() const;

public Q_SLOTS:
    void         setEnabled(bool enabled = true);
    void         setDisabled(bool disabled = true);

Q_SIGNALS:
    void         sig_activated();

private:
    Q_OBJECT
    QTLIB_DECLARE_PRIVATE(GlobalShortcut)

    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)
    Q_PROPERTY(QKeySequence shortcut READ get WRITE set)
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
