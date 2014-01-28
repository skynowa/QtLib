/**
 * \file   CUtils.h
 * \brief
 */


#ifndef QtLib_CUtilsH
#define QtLib_CUtilsH
//-------------------------------------------------------------------------------------------------
#include "Common.h"
//-------------------------------------------------------------------------------------------------
class CUtils
{
private:
                 CUtils();
    virtual     ~CUtils();

public:
    // GUI
    static
    bool         setApplicationSingle(cQString &applicationGuid);
        ///< set application single inststance
    static
    void         widgetAlignCenter(QWidget *widget);
        ///< align main widget
    static
    void         widgetAlignTopCenter(QWidget *widget);
        ///< align main widget
    static
    void         applicationActivate(cQString &className, cQString &windowName);
        ///< activate main application window
    static
    void         widgetAlwaysOnTop(QWidget *widget, cbool &isChecked);
        ///< toggle 'always on top' for a QMainWindow


    // DB
    typedef QVector< QPair<QString, QString> > db_fields_t;
    typedef const db_fields_t                  cdb_fields_t;
        ///< list of db fields

    static
    int          sqlTableModelRowCount(QSqlTableModel *model, cbool &isFetchAllRows = true);
        ///< DB row count
    static
    void         importCsv(cQString &filePath, QSqlTableModel *sqlTableModel,
                     const QVector<QString> &fieldNames, cQString &columnSeparator);
        ///< import DB to CSV file
    static
    void         exportCsv(cQString &filePath, QSqlTableModel *sqlTableModel,
                     const QVector<QString> &fieldNames, cQString &columnSeparator);
        ///< export CSV file to DB
    static
    void         dbFilter(QSqlQueryModel *sqlQueryModel, cQString &tableName, cdb_fields_t &fields,
                     cQString &sqlStrJoin, cQString &sqlStrWhere, cQString &sqlStrOrderBy);
        ///< DB filter
    static
    void         dbFieldNames(const QSqlDatabase &db, cQString &tableName,
                     QStringList *dbFileldNames);

        ///< DB filed names


    // web
    static
    QString      googleTranslate(cQString &textFrom, cQString &langFrom, cQString &langTo);
        ///< translate text by Google API


    // grafics
    static
    void         imageConvert(cQString &filePathIn, QByteArray *photo, cQByteArray &format = "JPEG");
        ///< convert images


    // converters
    static
    std::wstring toStdWString(cQString &str);
        ///< convert a QString to an std::wstring
    static
    QString      fromStdWString(std_cwstring &str);
        ///< convert an std::wstring to a QString


    // formaters
    static
    QString      formatBytes(culonglong &bytes);
        ///< format bytes


    // debug
    static
    void         debugTest(cQString &expression, cQString &filePath, culong &fileLine,
                     cQString &functionName, cqint64 &logSizeMaxBytes);
        ///< custom assert
    static
    void         debugTracer(QtMsgType type,
                            const QMessageLogContext &context, cQString &msg);
        ///< callback for qInstallMessageHandler


    // other
    static
    void         sleep(cint &timeoutMsec);
};
//-------------------------------------------------------------------------------------------------
#endif // QtLib_CUtilsH
