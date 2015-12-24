/**
 * \file   SqlTableModel.h
 * \brief
 */


#pragma once

#include "../Common.h"
#include "../Utils.h"
//-------------------------------------------------------------------------------------------------
namespace qtlib
{

class SqlTableModel :
    public QSqlTableModel
{
public:
    explicit     SqlTableModel(QObject *a_parent = Q_NULLPTR, QSqlDatabase a_db = QSqlDatabase());
    virtual     ~SqlTableModel() {}

    virtual bool select() Q_DECL_OVERRIDE;

Q_SIGNALS:
    void         sig_selectProgress(int value);

private:
#if defined(QT_DEBUG)
    size_t       _selectCount;
#endif

    Q_OBJECT
    Q_DISABLE_COPY(SqlTableModel)
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
