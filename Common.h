/**
 * \file  Common.h
 * \brief QtLib common header
 */


#ifndef QtLib_CommonH
#define QtLib_CommonH
//-------------------------------------------------------------------------------------------------
// #define qTEST_IS_USED  1

// core
#include <Qt>
#include <QtGlobal>
#include <QTextCodec>
#include <QClipboard>
#include <QSignalMapper>
#include <QTimer>
#include <QBuffer>
#include <QLocale>

// GUI
#include <QtGui>
#include <QCoreApplication>
#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QMainWindow>
#include <QDesktopWidget>
#include <QWidget>
#include <QShortcut>
#include <QFileDialog>
#include <QInputDialog>
#include <QLabel>
#include <QMenu>
#include <QSystemTrayIcon>

// containers
#include <QVector>
#include <QList>
#include <QStringList>
#include <QSettings>

// filesystem
#include <QDir>
#include <QFile>

// threads


// SQL
#include <QtSql>
#include <QSqlTableModel>

// DB
#include <QDataWidgetMapper>

// table
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QStandardItemModel>
#include <QTableView>

// network
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>

// translate
#include <QTranslator>

// etc
#include <QImage>
#include <QProxyStyle>
#include <QFileIconProvider>
#include <QDesktopServices>

// std
#include <iostream>

//-------------------------------------------------------------------------------------------------
// typedefs
// namespace qtlib {

typedef const bool               cbool;
typedef const int                cint;
typedef const long               clong;
typedef const long long          clonglong;
typedef const unsigned int       cuint;
typedef const unsigned long      culong;
typedef const unsigned long long culonglong;
typedef const float              cfloat;
typedef const double             cdouble;

typedef const qint64             cqint64;
typedef const QChar              cQChar;
typedef const QString            cQString;
typedef const QStringList        cQStringList;
typedef const QByteArray         cQByteArray;
typedef const QVariant           cQVariant;
typedef const QSize              cQSize;

// } // namespace qtlib
//-------------------------------------------------------------------------------------------------
#if defined(Q_OS_WIN)
    #ifdef QT_PLUGIN
        #define qPLUGIN_EXPORT Q_DECL_EXPORT
    #else
        #define qPLUGIN_EXPORT Q_DECL_IMPORT
    #endif
#else
    #define qPLUGIN_EXPORT
#endif
//-------------------------------------------------------------------------------------------------
#define qCHECK_RET(expr, return_expr) \
            { if ((expr)) { return (return_expr); } }
#define qCHECK_DO(expr, do_expr) \
            { if ((expr)) { do_expr;              } }

#define qNOT_IMPL \
            { QMessageBox::warning(this, qApp->applicationName(), QObject::tr(__FUNCTION__) + QObject::tr(": Not implemented")); }
#define qMSG_INFO(s) \
            { QMessageBox::information(this, qApp->applicationName(), (s)); }
#define qMSG_WARN(s) \
            { QMessageBox::warning(this, qApp->applicationName(), (s)); }
#define qMSG(s) \
            { QMessageBox::information(Q_NULLPTR, qApp->applicationName(), (s)); }

#define qCHECK_REF(var, object) \
            { \
                if (!var) { \
                    QMessageBox::critical(0, qApp->applicationName(), object.lastError().text(),  QMessageBox::Ok); \
                }\
            }
#define qCHECK_PTR(var, object) \
            { \
                if (!var) { \
                    QMessageBox::critical(0, qApp->applicationName(), object->lastError().text(), QMessageBox::Ok); \
                } \
            }
//-------------------------------------------------------------------------------------------------
// converters
#if defined(UNICODE) || defined(_UNICODE)
    #define qS2QS(s) \
            ( CUtils::fromStdWString(s) )
        ///< convert std::tstring_t to QString
    #define qQS2S(qs) \
            ( CUtils::toStdWString(qs) )
        ///< convert QString to std::tstring_t
#else
    #define qS2QS(s) \
            ( QString().fromStdString(s) )
        ///< convert std::tstring_t to QString
    #define qQS2S(qs) \
            ( (qs).toStdString() )
        ///< convert QString to std::tstring_t
#endif
//-------------------------------------------------------------------------------------------------
// debug variables, trace variable and value
#define  qDEBUG_VAR(v) \
                #v":" << (v)
//-------------------------------------------------------------------------------------------------
// custom assert
#if qTEST_IS_USED
    #define qTEST(expression) \
        if (!(expression)) { \
            cqint64 mByte = 1000 * 1000; \
            qtlib::Utils::debugTest(#expression, __FILE__, __LINE__, __FUNCTION__, mByte); \
            Q_ASSERT(expression); \
            std::exit(EXIT_FAILURE); \
        }

    #define qTEST_PTR(expression) \
            qTEST((expression) != Q_NULLPTR)
#else
    #define qTEST(expression) \
                Q_ASSERT(expression)

    #define qTEST_PTR(expression) \
                Q_ASSERT((expression) != Q_NULLPTR)
#endif

#define qTEST_NA(expression) \
            Q_UNUSED(expression);
//-------------------------------------------------------------------------------------------------
// etc
#define qARRAY_LENGTH(a) ( sizeof(a) / sizeof((a)[0]) )
#define qPTR_DELETE(p)   { delete p; p = Q_NULLPTR; }
//-------------------------------------------------------------------------------------------------
#endif // QtLib_CommonH
