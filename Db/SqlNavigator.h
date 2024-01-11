/**
 * \file  SqlNavigator.h
 * \brief
 */


#pragma once

#include "../Common.h"
#include "SqlRelationalTableModelEx.h"
//-------------------------------------------------------------------------------------------------
namespace qtlib
{

class SqlNavigator :
    public QObject
    /// DB SQL navigator
{
public:
///@name ctors, dtor
///@{
    explicit  SqlNavigator(QObject *parent);
    virtual  ~SqlNavigator();

    Q_DISABLE_COPY(SqlNavigator)

    void      construct(SqlRelationalTableModelEx *tableModel, QTableView *tableView);
///@}

    SqlRelationalTableModelEx *model();
    QTableView *view();
    bool        isValid() const;

    void        first();
    void        prior();
    void        next();
    void        last();
    void        goTo(cint rowIndex);

    void        insert();
    void        remove();
    void        edit();

private:
    SqlRelationalTableModelEx *_model {};
    QTableView                *_view {};
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
