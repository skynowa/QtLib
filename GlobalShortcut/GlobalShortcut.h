/**
 * \file
 * \brief
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
    explicit     GlobalShortcut(QObject *parent /* = Q_NULLPTR */);
    explicit     GlobalShortcut(const QKeySequence &shortcut, QObject *parent /* = Q_NULLPTR */);
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
