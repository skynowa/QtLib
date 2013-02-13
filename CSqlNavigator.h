/**
 * \file   CSqlNavigator.h
 * \brief
 */


#ifndef QtLib_CSqlNavigatorH
#define QtLib_CSqlNavigatorH
//-----------------------------------------------------------------------------
#include "Common.h"
//-----------------------------------------------------------------------------
class CSqlNavigator :
    public QObject
{
    Q_OBJECT

public:
    explicit        CSqlNavigator(QWidget        *parent,
                                  QSqlTableModel *tableModel = NULL,
                                  QTableView     *tableView  = NULL);
    virtual        ~CSqlNavigator();

    void            construct    (QSqlTableModel *tableModel,
                                  QTableView     *tableView);

    QSqlTableModel *model        ();
    QTableView     *view         ();
    bool            isValid      () const;

    void            first        ();
    void            prior        ();
    void            next         ();
    void            last         ();
    void            to           (cint &rowIndex);

    void            insert       ();
    void            remove       ();
    void            edit         ();
    void            post         ();
    void            cancel       ();
    void            refresh      ();

private:
    QSqlTableModel *_m_tmModel;
    QTableView     *_m_tvView;

};
//-----------------------------------------------------------------------------
#endif // QtLib_CSqlNavigatorH
