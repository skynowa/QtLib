/**
 * \file
 * \brief
 */


#ifndef QXTGLOBALSHORTCUT_H
#define QXTGLOBALSHORTCUT_H
//-------------------------------------------------------------------------------------------------
#include <QObject>
#include <QKeySequence>

#include "QxtGlobal.h"
//-------------------------------------------------------------------------------------------------
namespace qtlib {

class GlobalShortcut_impl;

class GlobalShortcut :
    public QObject
{
public:
    explicit GlobalShortcut(QObject* parent = Q_NULLPTR);
    explicit GlobalShortcut(const QKeySequence& shortcut, QObject* parent = Q_NULLPTR);
    virtual ~GlobalShortcut();

    QKeySequence shortcut() const;
    bool setShortcut(const QKeySequence& shortcut);

    bool isEnabled() const;

public Q_SLOTS:
    void setEnabled(bool enabled = true);
    void setDisabled(bool disabled = true);

Q_SIGNALS:
    void activated();

private:
    Q_OBJECT
    QXT_DECLARE_PRIVATE(GlobalShortcut)

    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)
    Q_PROPERTY(QKeySequence shortcut READ shortcut WRITE setShortcut)
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
#endif // QXTGLOBALSHORTCUT_H
