/**
 * \file   SqlNavigator.h
 * \brief
 */


#pragma once

#include "Common.h"
//-------------------------------------------------------------------------------------------------
namespace qtlib
{

class SqlNavigator :
    public QObject
    /// DB SQL navigator
{
public:
    explicit        SqlNavigator(QWidget *parent);
    virtual        ~SqlNavigator();

    void            construct(QSqlTableModel *tableModel, QTableView *tableView);

    QSqlTableModel *model();
    QTableView     *view();
    bool            isValid() const;

    void            first();
    void            prior();
    void            next();
    void            last();
    void            goTo(cint &rowIndex);

    void            insert();
    void            remove();
    void            edit();

private:
    QSqlTableModel *_model;
    QTableView     *_view;

    Q_DISABLE_COPY(SqlNavigator)
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
