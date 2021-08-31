/**
 * \file   SqlNavigator.h
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
    explicit  SqlNavigator(QWidget *parent);
    virtual  ~SqlNavigator();

    void      construct(SqlRelationalTableModelEx *tableModel, QTableView *tableView);

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

    Q_DISABLE_COPY(SqlNavigator);
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
