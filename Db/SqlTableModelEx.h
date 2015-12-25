/**
 * \file   SqlTableModelEx.h
 * \brief
 */


#pragma once

#include "../Common.h"
#include "../Utils.h"
//-------------------------------------------------------------------------------------------------
namespace qtlib
{

class SqlTableModelEx :
    public QSqlRelationalTableModel
{
public:
    explicit     SqlTableModelEx(QObject *parent = Q_NULLPTR, QSqlDatabase db = QSqlDatabase());
    virtual     ~SqlTableModelEx() {}

    virtual bool select() Q_DECL_OVERRIDE;

Q_SIGNALS:
    void         sig_selectProgress(int value);

private:
#if defined(QT_DEBUG)
    size_t       _selectCount;
#endif

    Q_OBJECT
    Q_DISABLE_COPY(SqlTableModelEx)
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
