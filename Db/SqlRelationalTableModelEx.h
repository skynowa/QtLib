/**
 * \file  SqlRelationalTableModelEx.h
 * \brief
 */


#pragma once

#include "../Common.h"
#include "../Utils.h"
//-------------------------------------------------------------------------------------------------
namespace qtlib
{

class SqlRelationalTableModelEx :
    public QSqlRelationalTableModel
{
public:
///@name ctors, dtor
///@{
    explicit  SqlRelationalTableModelEx(QObject *parent, QSqlDatabase db);
    virtual  ~SqlRelationalTableModelEx() {}

    Q_DISABLE_COPY(SqlRelationalTableModelEx)
///@}

    bool select() final;

    int  realRowCount();
        ///< get real rows count
    void importCsv(cQString &filePath, const QVector<QString> &fieldNames, cQString &csvSeparator,
            cbool isNormalize, QString *infoMsg);
    void importCsvClipboard(const QVector<QString> &fieldNames, cQString &csvSeparator,
            cbool isNormalize, QString *infoMsg);
        ///< import to CSV file
    void exportCsv(cQString &filePath, const QVector<QString> &fieldNames, cQString &csvSeparator,
            cbool isNormalize);
        ///< export from CSV file

    using db_fields_t  = QVector< QPair<QString, QString> >;
    using cdb_fields_t = const db_fields_t;

    void filter(cdb_fields_t &fields, cQString &join, cQString &where, cQString &orderBy);
        ///< filter

Q_SIGNALS:
    void sig_selectProgress(int value);

private:
    Q_OBJECT

#if defined(QT_DEBUG)
    size_t _selectCount {};
#endif

    void _csvNormalize(QString *fieldName, QString *fieldValue);
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
