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

#define qTYPEDEF_CONST(t) \
    typedef const t c##t
    ///< typedef const types

// namespace qtlib
// {

qTYPEDEF_CONST(bool);
qTYPEDEF_CONST(int);
qTYPEDEF_CONST(char);
qTYPEDEF_CONST(long);
typedef const long long          clonglong;
typedef const unsigned int       cuint;
typedef const unsigned long      culong;
typedef const unsigned long long culonglong;
qTYPEDEF_CONST(float);
qTYPEDEF_CONST(double);

qTYPEDEF_CONST(qint64);
qTYPEDEF_CONST(QChar);
qTYPEDEF_CONST(QString);
qTYPEDEF_CONST(QStringList);
qTYPEDEF_CONST(QByteArray);
qTYPEDEF_CONST(QVariant);
qTYPEDEF_CONST(QSize);
qTYPEDEF_CONST(QModelIndex);
qTYPEDEF_CONST(QUrl);
qTYPEDEF_CONST(QIcon);
qTYPEDEF_CONST(QSqlDatabase);

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
            { QMessageBox::information(Q_NULLPTR, qApp->applicationName(), (s)); }

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
