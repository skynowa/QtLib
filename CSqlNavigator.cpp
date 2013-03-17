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
    QWidget        *a_parent,
    QSqlTableModel *a_tableModel /* = NULL */,
    QTableView     *a_tableView  /* = NULL */
) :
    QObject   (a_parent),
    _m_tmModel(a_tableModel),
    _m_tvView (a_tableView)
{
    Q_ASSERT(NULL != a_parent);
    // a_tableModel - n/a
    // a_tableView - n/a
}
//------------------------------------------------------------------------------
/* virtual */
CSqlNavigator::~CSqlNavigator() {

}
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
QSqlTableModel *
CSqlNavigator::model() {
    Q_ASSERT(NULL != _m_tmModel);

    return _m_tmModel;
}
//------------------------------------------------------------------------------
QTableView *
CSqlNavigator::view() {
    Q_ASSERT(NULL != _m_tvView);

    return _m_tvView;
}
//------------------------------------------------------------------------------
bool
CSqlNavigator::isValid() const {
    return ( (NULL != _m_tmModel) && (NULL != _m_tvView) );
}
//------------------------------------------------------------------------------
void
CSqlNavigator::first() {
    qCHECK_DO(!isValid(), return);

    cint ciTargetRow = 0;

    goTo(ciTargetRow);
}
//------------------------------------------------------------------------------
void
CSqlNavigator::prior() {
    qCHECK_DO(!isValid(), return);

    cint ciTargetRow = view()->currentIndex().row() - 1;

    goTo(ciTargetRow);
}
//------------------------------------------------------------------------------
void
CSqlNavigator::next() {
    qCHECK_DO(!isValid(), return);

    cint ciTargetRow = view()->currentIndex().row() + 1;

    goTo(ciTargetRow);
}
//------------------------------------------------------------------------------
void
CSqlNavigator::last() {
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
    qCHECK_DO(rowIndex < 0, rowIndex = 0);

    for ( ; model()->canFetchMore(); ) {
        model()->fetchMore();
    }

    qCHECK_DO(rowIndex > model()->rowCount() - 1, rowIndex = model()->rowCount() - 1);

    view()->setFocus();
    view()->selectRow(rowIndex);
}
//------------------------------------------------------------------------------
void
CSqlNavigator::insert() {
    qCHECK_DO(!isValid(), return);

    bool bRv = model()->insertRow(
                    CUtils::sqlTableModelRowCount(model()), QModelIndex());
    qCHECK_PTR(bRv, model());

    last();
}
//------------------------------------------------------------------------------
void
CSqlNavigator::remove() {
    qCHECK_DO(!isValid(), return);

    QModelIndexList ilList = view()->selectionModel()->selectedIndexes();
    foreach (QModelIndex index, ilList) {
        cint ciTargetRow = index.row();

        view()->setFocus();
        bool bRv = model()->removeRow(ciTargetRow, QModelIndex());
        qCHECK_PTR(bRv, model());
    }
}
//------------------------------------------------------------------------------
void
CSqlNavigator::edit() {
    qCHECK_DO(!isValid(), return);

    cint              ciTargetCell = 1;

    cint              ciTargetRow  = view()->currentIndex().row();
    const QModelIndex cmiIndex     = model()->index(ciTargetRow, ciTargetCell);
    qCHECK_DO(- 1 == ciTargetRow, return);

    goTo(cmiIndex.row());
    view()->edit(cmiIndex);
}
//------------------------------------------------------------------------------
void
CSqlNavigator::post() {
    qCHECK_DO(!isValid(), return);

    cint              ciTargetCell = 1;

    cint              ciTargetRow  = view()->currentIndex().row();
    const QModelIndex cmiIndex     = model()->index(ciTargetRow, ciTargetCell);

    bool bRv = model()->submitAll();
    qCHECK_PTR(bRv, model());

    view()->setFocus();
    view()->setCurrentIndex(cmiIndex);
    view()->update(cmiIndex);
}
//------------------------------------------------------------------------------
void
CSqlNavigator::cancel() {
    qCHECK_DO(!isValid(), return);

    model()->revertAll();
    view()->setFocus();
}
//------------------------------------------------------------------------------
void
CSqlNavigator::refresh() {
    qCHECK_DO(!isValid(), return);

    cint ciTargetRow = view()->currentIndex().row();
    qCHECK_DO(- 1 == ciTargetRow, return);

    bool bRv = model()->select();
    qCHECK_PTR(bRv, model());

    view()->setFocus();
    view()->selectRow(ciTargetRow);
}
//------------------------------------------------------------------------------
