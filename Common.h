/**
 * \file  Common.h
 * \brief common header
 */


#ifndef QT_COMMON_H
#define QT_COMMON_H
//---------------------------------------------------------------------------
// Core
#include <Qt>
#include <QtGlobal>
#include <QTextCodec>
#include <QDebug>
#include <QClipboard>
#include <QSignalMapper>
#include <QTimer.h>

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

// Containers
#include <QVector>
#include <QList>
#include <QStringList>
#include <QSettings>

// filesystem
#include <QDir>
#include <QFile>

// Sql
#include <QtSql>
#include <QSqlTableModel>

// Table
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QStandardItemModel>
#include <QTableView>

// Network
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>

// Other
#include <QImage>

// xLib
#include <xLib/Common/xCommon.h>


//---------------------------------------------------------------------------
#define qNOT_IMPL               { QMessageBox::warning(this, qApp->applicationName(), tr(__FUNCTION__) + tr(": Not implemented")); }
#define qMSG(s)                 { QMessageBox::information(NULL, qApp->applicationName(), s); }
#define qCHECK_REF(var, object) { if (false == var) { QMessageBox::critical(0, qApp->applicationName(), object.lastError().text(),  QMessageBox::Ok); } }
#define qCHECK_PTR(var, object) { if (false == var) { QMessageBox::critical(0, qApp->applicationName(), object->lastError().text(), QMessageBox::Ok); } }

//---------------------------------------------------------------------------
// converters
#define qS2QS(s)                ( QString((s).c_str()) )
    ///< convert std::tstring_t to QString
#define qQS2S(qs)               ( std::tstring_t( (qs).begin(), (qs).begin() + (qs).size() ) )
    ///< convert QString to std::tstring_t

//---------------------------------------------------------------------------
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

//---------------------------------------------------------------------------
// other


//---------------------------------------------------------------------------
#endif // QT_COMMON_H
