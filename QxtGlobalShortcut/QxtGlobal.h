/**
 * \file
 * \brief
 */


#ifndef QXTGLOBAL_H
#define QXTGLOBAL_H
//-------------------------------------------------------------------------------------------------
#include <QtGlobal>
//-------------------------------------------------------------------------------------------------
#define QXT_DECLARE_PRIVATE(PUB) friend class PUB##Private; QxtPrivateInterface<PUB, PUB##Private> qxt_d;
#define QXT_DECLARE_PUBLIC(PUB)  friend class PUB;
#define QXT_INIT_PRIVATE(PUB)    qxt_d.setPublic(this);
#define QXT_D(PUB)               PUB##Private& d = qxt_d()
#define QXT_P(PUB)               PUB& p = qxt_p()
//-------------------------------------------------------------------------------------------------
template <typename PUB>
class QxtPrivate
{
public:
    virtual ~QxtPrivate()
    {
    }
    void
    QXT_setPublic(PUB* pub)
    {
        qxt_p_ptr = pub;
    }

protected:
    PUB&
    qxt_p()
    {
        return *qxt_p_ptr;
    }
    const PUB&
    qxt_p() const
    {
        return *qxt_p_ptr;
    }
    PUB*
    qxt_ptr()
    {
        return qxt_p_ptr;
    }
    const PUB*
    qxt_ptr() const
    {
        return qxt_p_ptr;
    }

private:
    PUB* qxt_p_ptr;
};
//-------------------------------------------------------------------------------------------------
template <typename PUB, typename PVT>
class QxtPrivateInterface
{
public:
    QxtPrivateInterface()
    {
        pvt = new PVT;
    }
    ~QxtPrivateInterface()
    {
        delete pvt; pvt = Q_NULLPTR;
    }

    void
    setPublic(PUB* pub)
    {
        pvt->QXT_setPublic(pub);
    }
    PVT&
    operator()()
    {
        return *static_cast<PVT*>(pvt);
    }
    const PVT&
    operator()() const
    {
        return *static_cast<PVT*>(pvt);
    }
    PVT *
    operator->()
    {
        return static_cast<PVT*>(pvt);
    }
    const PVT *
    operator->() const
    {
        return static_cast<PVT*>(pvt);
    }

private:
    QxtPrivate<PUB>* pvt;

    QxtPrivateInterface(const QxtPrivateInterface&)
    {
    }
    QxtPrivateInterface& operator=(const QxtPrivateInterface&)
    {
    }

    friend class QxtPrivate<PUB>;
};
//-------------------------------------------------------------------------------------------------
#endif // QXT_GLOBAL
