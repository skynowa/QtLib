/**
 * \file   SqlTableModel.cpp
 * \brief
 */


#include "SqlTableModel.h"

#include "Utils.h"


namespace qtlib
{

/**************************************************************************************************
*   public
*
**************************************************************************************************/

//-------------------------------------------------------------------------------------------------
SqlTableModel::SqlTableModel(
    QObject     *a_parent /* = Q_NULLPTR */,
    QSqlDatabase a_db     /* = QSqlDatabase() */
) :
    QSqlTableModel(a_parent, a_db)
#if defined(QT_DEBUG)
    ,
    _selectCount  (0)
#endif
{
}
//-------------------------------------------------------------------------------------------------
/* virtual */
bool
SqlTableModel::select()
{
#if defined(QT_DEBUG)
    ++ _selectCount;

    if (_selectCount > 1) {
        qWarning() << "Warning: SqlTableModel::" << __FUNCTION__ << ": " << qTRACE_VAR(_selectCount);
    }
#endif

    bool bRv = false;

    Q_EMIT sig_selectProgress( rowCount() );

    bRv = QSqlTableModel::select();
    if (!bRv) {
        return false;
    }

    Q_EMIT sig_selectProgress( rowCount() );

    for ( ; ; ) {
        bRv = QSqlQueryModel::canFetchMore();
        if (!bRv) {
            break;
        }

        QSqlQueryModel::fetchMore();

        Q_EMIT sig_selectProgress( rowCount() );
    }

    return true;
}
//-------------------------------------------------------------------------------------------------

} // namespace qtlib
