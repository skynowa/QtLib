/**
 * \file
 * \brief
 */


#ifndef QXTGLOBAL_H
#define QXTGLOBAL_H
//-------------------------------------------------------------------------------------------------
#include <QtGlobal>
#include <QtDebug>
#include <QVector>
#include <QMap>
#include <QHash>
#include <QApplication>
//-------------------------------------------------------------------------------------------------
#define QXT_DECLARE_PRIVATE(pub) \
    friend class pub##Private; \
    PrivateInterface<pub, pub##Private> _privateInterface;

#define QXT_DECLARE_PUBLIC(pub) \
    friend class pub;

#define QXT_INIT_PRIVATE(pub)    \
    _privateInterface.setPublic(this);

#define QXT_D(pub) \
    pub##Private& d = _privateInterface()

#define QXT_P(pub) \
    pub& p = get()
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
template <typename PublicT, typename PrivateInterfaceT>
class PrivateInterface
{
public:
    PrivateInterface()
    {
        _interface = new PrivateInterfaceT;
    }
    ~PrivateInterface()
    {
        delete _interface; _interface = Q_NULLPTR;
    }

    void
    setPublic(PublicT* pub)
    {
        _interface->setPublic(pub);
    }
    PrivateInterfaceT&
    operator () ()
    {
        return *static_cast<PrivateInterfaceT *>(_interface);
    }
    const PrivateInterfaceT&
    operator () () const
    {
        return *static_cast<PrivateInterfaceT *>(_interface);
    }

private:
    Private<PublicT>* _interface;

    PrivateInterface(const PrivateInterface&)
    {
    }
    PrivateInterface& operator = (const PrivateInterface&)
    {
    }

    friend class Private<PublicT>;
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
#endif // QXT_GLOBAL
