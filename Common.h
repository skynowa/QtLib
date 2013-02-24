/**
 * \file  Common.h
 * \brief QtLib common header
 */


#ifndef QtLib_CommonH
#define QtLib_CommonH
//------------------------------------------------------------------------------
// core
#include <Qt>
#include <QtGlobal>
#include <QTextCodec>
#include <QDebug>
#include <QClipboard>
#include <QSignalMapper>
#include <QTimer>
#include <QBuffer>

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

// etc
#include <QImage>
#include <QProxyStyle>
#include <QFileIconProvider>
#include <QDesktopServices>

//------------------------------------------------------------------------------
// typedefs
typedef const bool               cbool;
typedef const int                cint;
typedef const long               clong;
typedef const long long          clonglong;
typedef const unsigned int       cuint;
typedef const unsigned long      culong;
typedef const unsigned long long culonglong;
typedef const size_t             csize_t;
typedef const float              cfloat;
typedef const double             cdouble;
typedef const QString            cQString;
typedef const QStringList        cQStringList;
typedef const QByteArray         cQByteArray;
typedef const QVariant           cQVariant;
typedef const QSize              cSize;
typedef const std::string        std_cstring;
typedef const std::wstring       std_cwstring;
//------------------------------------------------------------------------------
#define qCHECK_RET(expr, return_expr)   { if ((expr)) { return (return_expr); } }
#define qCHECK_DO(expr, do_expr)        { if ((expr)) { do_expr;              } }

#define qNOT_IMPL                       { QMessageBox::warning(this, qApp->applicationName(), tr(__FUNCTION__) + tr(": Not implemented")); }
#define qMSG(s)                         { QMessageBox::information(NULL, qApp->applicationName(), s); }

#define qCHECK_REF(var, object)         { if (false == var) { QMessageBox::critical(0, qApp->applicationName(), object.lastError().text(),  QMessageBox::Ok); } }
#define qCHECK_PTR(var, object)         { if (false == var) { QMessageBox::critical(0, qApp->applicationName(), object->lastError().text(), QMessageBox::Ok); } }
//------------------------------------------------------------------------------
// converters
#if defined(UNICODE) || defined(_UNICODE)
    #define qS2QS(s)                    ( CUtils::fromStdWString(s) )
        ///< convert std::tstring_t to QString
    #define qQS2S(qs)                   ( CUtils::toStdWString(qs) )
        ///< convert QString to std::tstring_t
#else
    #define qS2QS(s)                    ( QString().fromStdString(s) )
        ///< convert std::tstring_t to QString
    #define qQS2S(qs)                   ( (qs).toStdString() )
        ///< convert QString to std::tstring_t
#endif
//------------------------------------------------------------------------------
// debug variables, trace variable and value
#define  qDEBUG_VAR(v) \
                #v":" << (v)

#define  qDEBUG_2VARS(_1, _2) \
                qDEBUG_VAR(_1) << qDEBUG_VAR(_2)

#define  qDEBUG_3VARS(_1, _2, _3) \
                qDEBUG_VAR(_1) << qDEBUG_VAR(_2) << qDEBUG_VAR(_3)

#define  qDEBUG_4VARS(_1, _2, _3, _4) \
                qDEBUG_VAR(_1) << qDEBUG_VAR(_2) << qDEBUG_VAR(_3) << qDEBUG_VAR(_4)

#define  qDEBUG_5VARS(_1, _2, _3, _4, _5) \
                qDEBUG_VAR(_1) << qDEBUG_VAR(_2) << qDEBUG_VAR(_3) << qDEBUG_VAR(_4) << \
                qDEBUG_VAR(_5)

#define  qDEBUG_6VARS(_1, _2, _3, _4, _5, _6) \
                qDEBUG_VAR(_1) << qDEBUG_VAR(_2) << qDEBUG_VAR(_3) << qDEBUG_VAR(_4) << \
                qDEBUG_VAR(_5) << qDEBUG_VAR(_6)

#define  qDEBUG_7VARS(_1, _2, _3, _4, _5, _6, _7) \
                qDEBUG_VAR(_1) << qDEBUG_VAR(_2) << qDEBUG_VAR(_3) << qDEBUG_VAR(_4) << \
                qDEBUG_VAR(_5) << qDEBUG_VAR(_6) << qDEBUG_VAR(_7)

#define  qDEBUG_8VARS(_1, _2, _3, _4, _5, _6, _7, _8) \
                qDEBUG_VAR(_1) << qDEBUG_VAR(_2) << qDEBUG_VAR(_3) << qDEBUG_VAR(_4) << \
                qDEBUG_VAR(_5) << qDEBUG_VAR(_6) << qDEBUG_VAR(_7) << qDEBUG_VAR(_8)

#define  qDEBUG_9VARS(_1, _2, _3, _4, _5, _6, _7, _8, _9) \
                qDEBUG_VAR(_1) << qDEBUG_VAR(_2) << qDEBUG_VAR(_3) << qDEBUG_VAR(_4) << \
                qDEBUG_VAR(_5) << qDEBUG_VAR(_6) << qDEBUG_VAR(_7) << qDEBUG_VAR(_8) << \
                qDEBUG_VAR(_9)

#define qDEBUG_10VARS(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10) \
                qDEBUG_VAR(_1) << qDEBUG_VAR(_2) << qDEBUG_VAR(_3) << qDEBUG_VAR(_4) << \
                qDEBUG_VAR(_5) << qDEBUG_VAR(_6) << qDEBUG_VAR(_7) << qDEBUG_VAR(_8) << \
                qDEBUG_VAR(_9) << qDEBUG_VAR(_10)

#define qDEBUG_11VARS(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) \
                qDEBUG_VAR(_1) << qDEBUG_VAR(_2) << qDEBUG_VAR(_3) << qDEBUG_VAR(_4) << \
                qDEBUG_VAR(_5) << qDEBUG_VAR(_6) << qDEBUG_VAR(_7) << qDEBUG_VAR(_8) << \
                qDEBUG_VAR(_9) << qDEBUG_VAR(_10) << qDEBUG_VAR(_11)

#define qDEBUG_12VARS(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) \
                qDEBUG_VAR(_1) << qDEBUG_VAR(_2) << qDEBUG_VAR(_3) << qDEBUG_VAR(_4) << \
                qDEBUG_VAR(_5) << qDEBUG_VAR(_6) << qDEBUG_VAR(_7) << qDEBUG_VAR(_8) << \
                qDEBUG_VAR(_9) << qDEBUG_VAR(_10) << qDEBUG_VAR(_11) << qDEBUG_VAR(_12)

//------------------------------------------------------------------------------
// etc


//------------------------------------------------------------------------------
#endif // QtLib_CommonH
