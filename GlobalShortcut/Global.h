/**
 * \file
 * \brief
 */


#pragma once

//-------------------------------------------------------------------------------------------------
#include <QtGlobal>
#include <QtDebug>
#include <QVector>
#include <QMap>
#include <QHash>
#include <QApplication>
//-------------------------------------------------------------------------------------------------
#define QTLIB_DECLARE_PRIVATE(pub) \
    friend class pub##Private; \
    Impl<pub, pub##_impl> _impl;

#define QTLIB_DECLARE_PUBLIC(pub) \
    friend class pub;

#define QTLIB_INIT_PRIVATE(pub) \
    _impl.setPublic(this);
//-------------------------------------------------------------------------------------------------
namespace qtlib {

template <typename PublicT>
class Private
{
public:
    virtual ~Private()
    {
    }
    void
    setPublic(PublicT* pub)
    {
        _pub = pub;
    }

protected:
    PublicT&
    get()
    {
        return *_pub;
    }
    const PublicT&
    get() const
    {
        return *_pub;
    }

private:
    PublicT* _pub;
};
//-------------------------------------------------------------------------------------------------
template <typename PublicT, typename ImplT>
class Impl
{
public:
    Impl()
    {
        _interface = new ImplT;
    }
    ~Impl()
    {
        delete _interface; _interface = Q_NULLPTR;
    }

    void
    setPublic(PublicT* pub)
    {
        _interface->setPublic(pub);
    }
    ImplT&
    operator () ()
    {
        return *static_cast<ImplT *>(_interface);
    }
    const ImplT&
    operator () () const
    {
        return *static_cast<ImplT *>(_interface);
    }

private:
    Private<PublicT>* _interface;

    Impl(const Impl&)
    {
    }
    Impl& operator = (const Impl&)
    {
    }

    friend class Private<PublicT>;
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
