/**
 * \file   CUtils.h
 * \brief
 */


#ifndef QtLib_CUtilsH
#define QtLib_CUtilsH
//---------------------------------------------------------------------------
#include "Common.h"
//---------------------------------------------------------------------------
class CUtils :
    public QObject
{
        Q_OBJECT

    public:
                            CUtils               ();
        virtual            ~CUtils               ();

        // GUI
        static bool         setApplicationSingle (const QString &applicationGuid);
            ///< set application single inststance
        static void         widgetAlignCenter    (QWidget *widget);
            ///< align main widget
        static void         widgetAlignTopCenter (QWidget *widget);
            ///< align main widget
        static void         applicationActivate  (const QString &className, const QString &windowName);
            ///< activate main application window


        // DB
        typedef QVector< QPair<QString, QString> > db_fields_t;
            ///< list of db fields

        static int          sqlTableModelRowCount(QSqlTableModel *model);
            ///< DB row count
        static void         importCsv            (const QString &filePath, QSqlTableModel *sqlTableModel,
                                                  const QVector<QString> &a_fieldNames, const QString &columnSeparator);
            ///< import DB to CSV file
        static void         exportCsv            (const QString &filePath, QSqlTableModel *sqlTableModel,
                                                  const QVector<QString> &a_fieldNames, const QString &columnSeparator);
            ///< export CSV file to DB
        static void         dbFilter             (QSqlQueryModel *sqlQueryModel, const QString &csTableName,
                                                  const db_fields_t &cFields, const QString &csSqlStrJoin,
                                                  const QString &csSqlStrWhere, const QString &csSqlStrOrderBy);
            ///< DB filter

        // web
        static QString      googleTranslate      (const QString &textFrom, const QString &langFrom, const QString &langTo);
            ///< translate text by Google API
        static void         googleSpeech         (const QString &text, const QString &lang, const QString &filePath);
            ///< speech text by Google API

        // grafics
        static void         imageConvert         (const QString    &filePathIn,
                                                  QByteArray       *baPhoto,
                                                  const QByteArray &format = "JPEG");
            ///< convert images

        // converters
        static std::wstring toStdWString         (const QString &str);
            ///< convert a QString to an std::wstring
        static QString      fromStdWString       (const std::wstring &str);
            ///< convert an std::wstring to a QString
};
//---------------------------------------------------------------------------
#endif // QtLib_CUtilsH
