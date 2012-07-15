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
#define qCHECK_REF(var, object) { if (false == var) { QMessageBox::critical(0, qApp->applicationName(), object.lastError().text(),  QMessageBox::Cancel); } }
#define qCHECK_PTR(var, object) { if (false == var) { QMessageBox::critical(0, qApp->applicationName(), object->lastError().text(), QMessageBox::Cancel); } }

//---------------------------------------------------------------------------
//converters
#define qS2QS(s)                ( QString((s).c_str()) )
    ///< convert std::tstring_t to QString
#define qQS2S(qs)               ( std::tstring_t( (qs).begin(), (qs).begin() + (qs).size() ) )
    ///< convert QString to std::tstring_t
//---------------------------------------------------------------------------
#endif // QT_COMMON_H
