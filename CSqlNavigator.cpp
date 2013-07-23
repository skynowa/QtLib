/**
 * \file   CSqlNavigator.cpp
 * \brief
 */


#include "CSqlNavigator.h"

#include "CUtils.h"


/*******************************************************************************
*   public
*
*******************************************************************************/

//------------------------------------------------------------------------------
CSqlNavigator::CSqlNavigator(
    QWidget *a_parent
) :
    QObject   (a_parent),
    _m_tmModel(NULL),
    _m_tvView (NULL)
{
    qTEST(NULL != a_parent);
}
//------------------------------------------------------------------------------
/* virtual */
CSqlNavigator::~CSqlNavigator()
{

}
//------------------------------------------------------------------------------
void
CSqlNavigator::construct(
    QSqlTableModel *a_tableModel,
    QTableView     *a_tableView
)
{
    qTEST(NULL != a_tableModel);
    qTEST(NULL != a_tableView);

    _m_tmModel = a_tableModel;
    _m_tvView  = a_tableView;
}
//------------------------------------------------------------------------------
QSqlTableModel *
CSqlNavigator::model()
{
    qTEST(NULL != _m_tmModel);

    return _m_tmModel;
}
//------------------------------------------------------------------------------
QTableView *
CSqlNavigator::view()
{
    qTEST(NULL != _m_tvView);

    return _m_tvView;
}
//------------------------------------------------------------------------------
bool
CSqlNavigator::isValid() const
{
    return ( (NULL != _m_tmModel) && (NULL != _m_tvView) );
}
//------------------------------------------------------------------------------
void
CSqlNavigator::first()
{
    qCHECK_DO(!isValid(), return);

    cint ciTargetRow = 0;

    goTo(ciTargetRow);
}
//------------------------------------------------------------------------------
void
CSqlNavigator::prior()
{
    qCHECK_DO(!isValid(), return);

    cint ciTargetRow = view()->currentIndex().row() - 1;

    goTo(ciTargetRow);
}
//------------------------------------------------------------------------------
void
CSqlNavigator::next()
{
    qCHECK_DO(!isValid(), return);

    cint ciTargetRow = view()->currentIndex().row() + 1;

    goTo(ciTargetRow);
}
//------------------------------------------------------------------------------
void
CSqlNavigator::last()
{
    qCHECK_DO(!isValid(), return);

    int iTargetRow = CUtils::sqlTableModelRowCount( model() ) - 1;
    qCHECK_DO(- 1 >= iTargetRow, iTargetRow = 0);

    goTo(iTargetRow);
}
//------------------------------------------------------------------------------
void
CSqlNavigator::goTo(
    cint &a_rowIndex
)
{
    // rowIndex - n/a

    qCHECK_DO(!isValid(), return);

    int rowIndex = a_rowIndex;
    {
        qCHECK_DO(rowIndex < 0, rowIndex = 0);

        // get real model()->rowCount()
        for ( ; model()->canFetchMore(); ) {
            model()->fetchMore();
        }

        if (rowIndex > model()->rowCount() - 1) {
            rowIndex = model()->rowCount() - 1;
        }
    }

    view()->setFocus();
    view()->selectRow(rowIndex);

    QModelIndex index = model()->index(rowIndex, 1);
    view()->scrollTo(index);
}
//------------------------------------------------------------------------------
void
CSqlNavigator::insert()
{
    qCHECK_DO(!isValid(), return);

    cint ciTargetField = 1;

    // empty record
    QSqlRecord record = model()->record();
    record.setValue(ciTargetField, QVariant());

    // if row is negative, the record will be appended to the end
    bool bRv = model()->insertRecord(- 1, record);
    qCHECK_PTR(bRv, model());

    bRv = model()->submitAll();
    qCHECK_PTR(bRv, model());

    bRv = model()->select();
    qCHECK_PTR(bRv, model());

    last();
}
//------------------------------------------------------------------------------
void
CSqlNavigator::remove()
{
    qCHECK_DO(!isValid(), return);

    int             iTargetRow = 0;
    QModelIndexList ilList     = view()->selectionModel()->selectedIndexes();

    Q_FOREACH (QModelIndex index, ilList) {
        iTargetRow = index.row();

        view()->setFocus();
        bool bRv = model()->removeRow(iTargetRow);
        qCHECK_PTR(bRv, model());
    }

    bool bRv = model()->submitAll();
    qCHECK_PTR(bRv, model());

    model()->select();
    goTo(iTargetRow - 1);
}
//------------------------------------------------------------------------------
void
CSqlNavigator::edit()
{
    qCHECK_DO(!isValid(), return);

    cint              ciTargetCell = 1;

    cint              ciTargetRow  = view()->currentIndex().row();
    const QModelIndex cmiIndex     = model()->index(ciTargetRow, ciTargetCell);
    qCHECK_DO(- 1 == ciTargetRow, return);

    bool bRv = model()->submitAll();
    qCHECK_PTR(bRv, model());

    goTo(cmiIndex.row());
}
//------------------------------------------------------------------------------
