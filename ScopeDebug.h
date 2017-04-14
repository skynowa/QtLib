/**
 * \file  ScopeLock.h
 * \brief
 */


#pragma once

//-------------------------------------------------------------------------------------------------
class ScopeDebug
{
public:
    explicit ScopeDebug(const QString &a_msg) :
        _msg(a_msg)
    {
        qDebug() << "\n\n::: Start " << _msg << " (" << QDateTime::currentDateTime().toString() << ") :::";
    }
   ~ScopeDebug()
    {
        qDebug() << "\n::: Finish " << _msg << " (" << QDateTime::currentDateTime().toString() << ") :::";
    }

private:
    const QString _msg;

    ScopeDebug(const ScopeDebug&) = delete;
    ScopeDebug& operator=(const ScopeDebug&) = delete;
};
//-------------------------------------------------------------------------------------------------
