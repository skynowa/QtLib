/**
 * \file  Common.h
 * \brief QtLib common header
 */


#pragma once

#define qTEST_IS_USED  1

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
#include <QStandardPaths>

// threads


// SQL
#include <QtSql>
#include <QSqlTableModel>

// DB
#include <QSqlDatabase>
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

#define qUSING_CONST(t) \
    using c##t = const t
    ///< typedef const types

// namespace qtlib
// {

qUSING_CONST(bool);
qUSING_CONST(int);
qUSING_CONST(char);
qUSING_CONST(long);
using clonglong  = const long long;
using cuint      = const unsigned int;
using culong     = const unsigned long;
using culonglong = const unsigned long long;
qUSING_CONST(float);
qUSING_CONST(double);

qUSING_CONST(qint64);
qUSING_CONST(QChar);
qUSING_CONST(QString);
qUSING_CONST(QStringList);
qUSING_CONST(QByteArray);
qUSING_CONST(QVariant);
qUSING_CONST(QSize);
qUSING_CONST(QModelIndex);
qUSING_CONST(QUrl);
qUSING_CONST(QIcon);
qUSING_CONST(QSqlDatabase);
qUSING_CONST(QSqlTableModel);

// } // namespace qtlib
//-------------------------------------------------------------------------------------------------
#if defined(Q_OS_WIN)
    #if defined(QT_PLUGIN)
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

#define qMSG_NOT_IMPL \
            { QMessageBox::warning(this, qApp->applicationName(), QObject::tr(__FUNCTION__) + QObject::tr(": Not implemented")); }
#define qMSG_INFO(s) \
            { QMessageBox::information(this, qApp->applicationName(), (s)); }
#define qMSG_WARN(s) \
            { QMessageBox::warning(this, qApp->applicationName(), (s)); }
#define qMSG(s) \
            { QMessageBox::information(nullptr, qApp->applicationName(), (s)); }

#define qCHECK_REF(var, object) \
            { \
                if (!var) { \
                    QMessageBox::critical(0, qApp->applicationName(), QObject::tr("Error: ") + object.lastError().text(),  QMessageBox::Ok); \
                }\
            }
#define qCHECK_PTR(var, object) \
            { \
                if (!var) { \
                    QMessageBox::critical(0, qApp->applicationName(), QObject::tr("Error: ") + object->lastError().text(), QMessageBox::Ok); \
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
#define qTRACE_VAR(v) \
            #v":" << (v)
#define qTRACE_FUNC \
            qDebug() << "::::: " << __FUNCTION__ << " :::::"
#define qTRACE_FUNC_VAR(v) \
            qDebug() << "\t::: " << __FUNCTION__ << ": " << qTRACE_VAR(v) << " :::"
#define qTRACE_POINT \
            qDebug() << "\t::: " << __FUNCTION__ << " (" << __LINE__ << ") :::"
//-------------------------------------------------------------------------------------------------
// custom assert
#if qTEST_IS_USED
    #define qTEST(expression) \
        if (!(expression)) { \
            cqint64 mByte = 1000 * 1000; \
            qtlib::Utils::debugTest(#expression, __FILE__, __LINE__, __FUNCTION__, mByte); \
            Q_ASSERT(expression); \
            /* std::exit(EXIT_FAILURE); */ \
        }

    #define qTEST_PTR(expression) \
            qTEST((expression) != nullptr)
#else
    #define qTEST(expression) \
                Q_ASSERT(expression)

    #define qTEST_PTR(expression) \
                Q_ASSERT((expression) != nullptr)
#endif

#define qTEST_NA(expression) \
            Q_UNUSED(expression);
//-------------------------------------------------------------------------------------------------
// etc
#define qARRAY_LENGTH(a) ( sizeof(a) / sizeof((a)[0]) )
#define qPTR_DELETE(p)   { delete p; p = nullptr; }
//-------------------------------------------------------------------------------------------------
