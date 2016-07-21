/**
 * \file   SqlNavigator.h
 * \brief
 */


#pragma once

#include "../Common.h"
#include "SqlTableModelEx.h"
//-------------------------------------------------------------------------------------------------
namespace qtlib
{

class SqlNavigator :
    public QObject
    /// DB SQL navigator
{
public:
    explicit         SqlNavigator(QWidget *parent);
    virtual         ~SqlNavigator();

    void             construct(SqlTableModelEx *tableModel, QTableView *tableView);

    SqlTableModelEx *model();
    QTableView      *view();
    bool             isValid() const;

    void             first();
    void             prior();
    void             next();
    void             last();
    void             goTo(cint &rowIndex);

    void             insert();
    void             remove();
    void             edit();

private:
    SqlTableModelEx *_model;
    QTableView      *_view;

    Q_DISABLE_COPY(SqlNavigator)
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
