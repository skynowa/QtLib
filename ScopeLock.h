/**
 * \file  ScopeLock.h
 * \brief
 */


#pragma once

//-------------------------------------------------------------------------------------------------
template <typename T>
class ScopeLock
{
public:
    explicit ScopeLock(const T &a_object) :
        _object(a_object)
    {
        bool bRv = _object.lock();
        STD_TEST_DO(bRv, qDebug() << STD_TRACE_VAR(_object.errorString()));
    }
    ScopeLock(const ScopeLock&) = delete;
    ScopeLock& operator=(const ScopeLock&) = delete;

   ~ScopeLock()
    {
        bool bRv = _object.unlock();
        STD_TEST_DO(bRv, qDebug() << STD_TRACE_VAR(_object.errorString()));
    }

private:
    const T &_object;
};
//-------------------------------------------------------------------------------------------------
