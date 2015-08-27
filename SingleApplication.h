/**
 * \file   SingleApplication.h
 * \brief
 */


#pragma once

#include <QApplication>
#include <QSharedMemory>
#include <QStringList>
#include "Common.h"
#include "QtLib/Utils.h"
//-------------------------------------------------------------------------------------------------
namespace qtlib
{

class SingleApplication :
    public QApplication
{
    Q_OBJECT
    Q_DISABLE_COPY(SingleApplication)

public:
    SingleApplication(int &argc, char *argv[], cQString &guid);

    bool isRunnig() const;
        ///< is application running
    bool isMaster() const;
    bool sendMessage(const QString &message) const;

public Q_SLOTS:
    void checkForMessage();

Q_SIGNALS:
    void messageAvailable(const QStringList &messages);

private:
    bool                  _is_running;
    mutable QSharedMemory _locker;
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
