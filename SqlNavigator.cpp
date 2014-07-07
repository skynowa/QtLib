/**
 * \file   SqlNavigator.cpp
 * \brief
 */


#include "SqlNavigator.h"

#include "Utils.h"


/**************************************************************************************************
*   public
*
**************************************************************************************************/

//-------------------------------------------------------------------------------------------------
SqlNavigator::SqlNavigator(
    QWidget *a_parent
) :
    QObject(a_parent),
    _model (Q_NULLPTR),
    _view  (Q_NULLPTR)
{
    qTEST_PTR(a_parent);
}
//-------------------------------------------------------------------------------------------------
/* virtual */
SqlNavigator::~SqlNavigator()
{
}
//-------------------------------------------------------------------------------------------------
void
SqlNavigator::construct(
    QSqlTableModel *a_tableModel,
    QTableView     *a_tableView
)
{
    qTEST_PTR(a_tableModel);
    qTEST_PTR(a_tableView);

    _model = a_tableModel;
    _view  = a_tableView;
}
//-------------------------------------------------------------------------------------------------
QSqlTableModel *
SqlNavigator::model()
{
    qTEST_PTR(_model);

    return _model;
}
//-------------------------------------------------------------------------------------------------
QTableView *
SqlNavigator::view()
{
    qTEST_PTR(_view);

    return _view;
}
//-------------------------------------------------------------------------------------------------
bool
SqlNavigator::isValid() const
{
    return ( (_model != Q_NULLPTR ) && (_view != Q_NULLPTR) );
}
//-------------------------------------------------------------------------------------------------
void
SqlNavigator::first()
{
    qCHECK_DO(!isValid(), return);

    cint targetRow = 0;

    goTo(targetRow);
}
//-------------------------------------------------------------------------------------------------
void
SqlNavigator::prior()
{
    qCHECK_DO(!isValid(), return);

    cint targetRow = view()->currentIndex().row() - 1;

    goTo(targetRow);
}
//-------------------------------------------------------------------------------------------------
void
SqlNavigator::next()
{
    qCHECK_DO(!isValid(), return);

    cint targetRow = view()->currentIndex().row() + 1;

    goTo(targetRow);
}
//-------------------------------------------------------------------------------------------------
void
SqlNavigator::last()
{
    qCHECK_DO(!isValid(), return);

    int targetRow = ::Utils::sqlTableModelRowCount( model() ) - 1;
    qCHECK_DO(- 1 >= targetRow, targetRow = 0);

    goTo(targetRow);
}
//-------------------------------------------------------------------------------------------------
void
SqlNavigator::goTo(
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
//-------------------------------------------------------------------------------------------------
void
SqlNavigator::insert()
{
    qCHECK_DO(!isValid(), return);

    cint targetField = 1;

    // empty record
    QSqlRecord record = model()->record();
    record.setValue(targetField, QVariant());

    // if row is negative, the record will be appended to the end
    bool bRv = model()->insertRecord(- 1, record);
    qCHECK_PTR(bRv, model());

    bRv = model()->submitAll();
    qCHECK_PTR(bRv, model());

    bRv = model()->select();
    qCHECK_PTR(bRv, model());

    last();
}
//-------------------------------------------------------------------------------------------------
void
SqlNavigator::remove()
{
    qCHECK_DO(!isValid(), return);

    int             targetRow = 0;
    QModelIndexList indexes   = view()->selectionModel()->selectedIndexes();

    Q_FOREACH (QModelIndex index, indexes) {
        targetRow = index.row();

        view()->setFocus();
        bool bRv = model()->removeRow(targetRow);
        qCHECK_PTR(bRv, model());
    }

    bool bRv = model()->submitAll();
    qCHECK_PTR(bRv, model());

    model()->select();
    goTo(targetRow - 1);
}
//-------------------------------------------------------------------------------------------------
void
SqlNavigator::edit()
{
    qCHECK_DO(!isValid(), return);

    cint              targetCell = 1;

    cint              targetRow = view()->currentIndex().row();
    const QModelIndex index     = model()->index(targetRow, targetCell);
    qCHECK_DO(- 1 == targetRow, return);

    bool bRv = model()->submitAll();
    qCHECK_PTR(bRv, model());

    goTo(index.row());
}
//-------------------------------------------------------------------------------------------------
