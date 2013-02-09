/**
 * \file   CSqlNavigator.cpp
 * \brief
 */


#include "CSqlNavigator.h"

#include "CUtils.h"


/****************************************************************************
*   public
*
*****************************************************************************/

//-----------------------------------------------------------------------------
CSqlNavigator::CSqlNavigator(
    QWidget        *a_parent,
    QSqlTableModel *a_tableModel /* = NULL */,
    QTableView     *a_tableView  /* = NULL */
) :
    QObject   (a_parent),
    _m_tmModel(a_tableModel),
    _m_tvView (a_tableView)
{
}
//-----------------------------------------------------------------------------
/* virtual */
CSqlNavigator::~CSqlNavigator() {

}
//-----------------------------------------------------------------------------
void
CSqlNavigator::construct(
    QSqlTableModel *a_tableModel,
    QTableView     *a_tableView
)
{
    Q_ASSERT(NULL != a_tableModel);
    Q_ASSERT(NULL != a_tableView);

    _m_tmModel = a_tableModel;
    _m_tvView  = a_tableView;
}
//-----------------------------------------------------------------------------
QSqlTableModel *
CSqlNavigator::model() {
    Q_ASSERT(NULL != _m_tmModel);

    return _m_tmModel;
}
//-----------------------------------------------------------------------------
QTableView *
CSqlNavigator::view() {
    Q_ASSERT(NULL != _m_tvView);

    return _m_tvView;
}
//-----------------------------------------------------------------------------
bool
CSqlNavigator::isValid() const {
    return ( (NULL != _m_tmModel) && (NULL != _m_tvView) );
}
//-----------------------------------------------------------------------------
void
CSqlNavigator::first() {
    qCHECK_DO(false == isValid(), return);

    int iTargetRow = 0;

    view()->setFocus();
    view()->selectRow(iTargetRow);
}
//-----------------------------------------------------------------------------
void
CSqlNavigator::prior() {
    qCHECK_DO(false == isValid(), return);

    int iTargetRow = view()->currentIndex().row() - 1;

    view()->setFocus();
    view()->selectRow(iTargetRow);
}
//-----------------------------------------------------------------------------
void
CSqlNavigator::next() {
    qCHECK_DO(false == isValid(), return);

    int iTargetRow = view()->currentIndex().row() + 1;

    view()->setFocus();
    view()->selectRow(iTargetRow);
}
//-----------------------------------------------------------------------------
void
CSqlNavigator::last() {
    qCHECK_DO(false == isValid(), return);

    int iTargetRow = CUtils::sqlTableModelRowCount( model() ) - 1;
    qCHECK_DO(- 1 >= iTargetRow, iTargetRow = 0);

    view()->setFocus();
    view()->selectRow(iTargetRow);
}
//-----------------------------------------------------------------------------
void
CSqlNavigator::to(
    const int &rowIndex
)
{
    qCHECK_DO(false == isValid(), return);

    for ( ; model()->canFetchMore(); ) {
        model()->fetchMore();
    }

    view()->setFocus();
    view()->selectRow(rowIndex);
}
//-----------------------------------------------------------------------------
void
CSqlNavigator::insert() {
    qCHECK_DO(false == isValid(), return);

    bool bRv = model()->insertRow(CUtils::sqlTableModelRowCount(model()), QModelIndex());
    qCHECK_PTR(bRv, model());

    last();
}
//-----------------------------------------------------------------------------
void
CSqlNavigator::remove() {
    qCHECK_DO(false == isValid(), return);

    QModelIndexList ilList = view()->selectionModel()->selectedIndexes();
    foreach (QModelIndex index, ilList) {
        int iTargetRow = index.row();

        view()->setFocus();
        bool bRv = view()->model()->removeRow(iTargetRow, QModelIndex());
        qCHECK_PTR(bRv, model());
    }
}
//-----------------------------------------------------------------------------
void
CSqlNavigator::edit() {
    qCHECK_DO(false == isValid(), return);

    const int   ciTargetCell = 1;

    int         iTargetRow   = view()->currentIndex().row();
    QModelIndex miIndex      = model()->index(iTargetRow, ciTargetCell);
    qCHECK_DO(- 1 == iTargetRow, return);

    view()->setFocus();
    view()->setCurrentIndex(miIndex);
    view()->edit(miIndex);
}
//-----------------------------------------------------------------------------
void
CSqlNavigator::post() {
    qCHECK_DO(false == isValid(), return);

    const int   ciTargetCell = 1;

    int         iTargetRow   = view()->currentIndex().row();
    QModelIndex miIndex      = model()->index(iTargetRow, ciTargetCell);

    bool bRv = model()->submitAll();
    qCHECK_PTR(bRv, model());

    view()->setFocus();
    view()->setCurrentIndex(miIndex);
    view()->update(miIndex);
}
//-----------------------------------------------------------------------------
void
CSqlNavigator::cancel() {
    qCHECK_DO(false == isValid(), return);

    model()->revertAll();
    view()->setFocus();
}
//-----------------------------------------------------------------------------
void
CSqlNavigator::refresh() {
    qCHECK_DO(false == isValid(), return);

    int iTargetRow = view()->currentIndex().row();
    qCHECK_DO(- 1 == iTargetRow, return);

    bool bRv = model()->select();
    qCHECK_PTR(bRv, model());

    view()->setFocus();
    view()->selectRow(iTargetRow);
}
//-----------------------------------------------------------------------------
