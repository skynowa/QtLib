/**
 * \file   Utils.h
 * \brief
 */


#pragma once

#include "Common.h"
//-------------------------------------------------------------------------------------------------
namespace qtlib
{

class Utils
    /// utils
{
public:
    // GUI
    static
    void         widgetAlignCenter(QWidget *widget);
        ///< align main widget
    static
    void         widgetAlignTopCenter(QWidget *widget);
        ///< align main widget
    static
    void         widgetAlwaysOnTop(QWidget *widget, cbool &isChecked);
        ///< toggle 'always on top' for a QMainWindow


    // web


    // grafics
    static
    void         imageConvert(cQString &filePathIn, QByteArray *photo, cQByteArray &format = "JPEG");
        ///< convert images


    // converters
    static
    std::wstring toStdWString(cQString &str);
        ///< convert a QString to an std::wstring
    static
    QString      fromStdWString(const std::wstring &str);
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
    void         debugTracer(QtMsgType type, const QMessageLogContext &context, cQString &msg);
        ///< callback for qInstallMessageHandler


    // other
    static
    void         sleepMsec(cint &timeoutMsec);

private:
                 Utils();
};

} // namespace qtlib
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
namespace qtlib
{

class AutoTrace
{
public:
    explicit AutoTrace(const char *a_str) :
        _str(a_str)
    {
        qDebug() << "\n\n::: Start " << _str << " (" << QDateTime::currentDateTime().toString() << ") :::";
    }
   ~AutoTrace()
    {
        qDebug() << "\n::: Finish " << _str << " (" << QDateTime::currentDateTime().toString() << ") :::";
    }

private:
    const char *_str;
};

#define qTRACE_SCOPE(s) qtlib::AutoTrace autoLog(s)
#define qTRACE_FUNCTION qTRACE_SCOPE(__FUNCTION__)

} // namespace qtlib
//-------------------------------------------------------------------------------------------------
