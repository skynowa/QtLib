/**
 * \file
 * \brief
 */


#ifndef QXTGLOBAL_H
#define QXTGLOBAL_H
//-------------------------------------------------------------------------------------------------
#include <QtGlobal>
//-------------------------------------------------------------------------------------------------
#define QXT_DECLARE_PRIVATE(pub) friend class pub##Private; QxtPrivateInterface<pub, pub##Private> qxt_d;
#define QXT_DECLARE_PUBLIC(pub)  friend class pub;
#define QXT_INIT_PRIVATE(pub)    qxt_d.setPublic(this);
#define QXT_D(pub)               pub##Private& d = qxt_d()
#define QXT_P(pub)               pub& p = get()
//-------------------------------------------------------------------------------------------------
namespace qtlib {

template <typename PublicT>
class QxtPrivate
{
public:
    virtual ~QxtPrivate()
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
class QxtPrivateInterface
{
public:
    QxtPrivateInterface()
    {
        _interface = new PrivateInterfaceT;
    }
    ~QxtPrivateInterface()
    {
        delete _interface; _interface = Q_NULLPTR;
    }

    void
    setPublic(PublicT* pub)
    {
        _interface->setPublic(pub);
    }
    PrivateInterfaceT&
    operator()()
    {
        return *static_cast<PrivateInterfaceT *>(_interface);
    }
    const PrivateInterfaceT&
    operator()() const
    {
        return *static_cast<PrivateInterfaceT *>(_interface);
    }

private:
    QxtPrivate<PublicT>* _interface;

    QxtPrivateInterface(const QxtPrivateInterface&)
    {
    }
    QxtPrivateInterface& operator = (const QxtPrivateInterface&)
    {
    }

    friend class QxtPrivate<PublicT>;
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
#endif // QXT_GLOBAL
