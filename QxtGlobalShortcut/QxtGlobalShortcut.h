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
class QxtGlobalShortcutPrivate;

class QxtGlobalShortcut :
    public QObject
{
public:
    explicit QxtGlobalShortcut(QObject* parent = Q_NULLPTR);
    explicit QxtGlobalShortcut(const QKeySequence& shortcut, QObject* parent = Q_NULLPTR);
    virtual ~QxtGlobalShortcut();

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
    QXT_DECLARE_PRIVATE(QxtGlobalShortcut)

    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)
    Q_PROPERTY(QKeySequence shortcut READ shortcut WRITE setShortcut)
};
//-------------------------------------------------------------------------------------------------
#endif // QXTGLOBALSHORTCUT_H
