/**
 * \file   SqlRelationalTableModelEx.h
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
    explicit     SqlRelationalTableModelEx(QObject *parent, QSqlDatabase db);
    virtual     ~SqlRelationalTableModelEx() {}

    bool         select() override;

    int          realRowCount();
        ///< get real rows count
    void         importCsv(cQString &filePath, const QVector<QString> &fieldNames,
                    cQString &csvSeparator, cbool isNormalize, QString *infoMsg);
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

    Q_OBJECT
    Q_DISABLE_COPY(SqlRelationalTableModelEx)

    void         _csvNormalize(QString *fieldName, QString *fieldValue);
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
