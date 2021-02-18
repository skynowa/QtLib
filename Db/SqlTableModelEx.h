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
    public QSqlTableModel
{
public:
    explicit     SqlTableModelEx(QObject *parent, QSqlDatabase db);
    virtual     ~SqlTableModelEx() {}

    bool         select() override;
        ///< select query

    int          realRowCount();
        ///< get real rows count
    void         importCsv(cQString &filePath, const QVector<QString> &fieldNames,
                    cQString &csvSeparator, cbool isNormalize);
        ///< import to CSV file
    void         exportCsv(cQString &filePath, const QVector<QString> &fieldNames,
                    cQString &csvSeparator, cbool isNormalize);
        ///< export from CSV file

    typedef QVector< QPair<QString, QString> > db_fields_t;
    typedef const db_fields_t                  cdb_fields_t;

    void         filter(cQString &tableName, cdb_fields_t &fields, cQString &join,
                    cQString &where, cQString &orderBy);
        ///< filter

Q_SIGNALS:
    void         sig_selectProgress(int value);

private:
#if defined(QT_DEBUG)
    size_t       _selectCount;
#endif

    void         _csvNormalize(QString *fieldName, QString *fieldValue);

    Q_OBJECT
    Q_DISABLE_COPY(SqlTableModelEx)
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
