#include "BuildInfo.h"

#include <QSysInfo>
#include <QtGlobal>

#include <climits>
#include <version>

namespace qtlib
{

QString
BuildInfo::configuration() const
{
#ifdef NDEBUG
    return QStringLiteral("Release");
#else
    return QStringLiteral("Debug");
#endif
}

QString
BuildInfo::dateTime() const
{
    return QStringLiteral(__DATE__ " " __TIME__);
}

QString
BuildInfo::languageStandard() const
{
    return QStringLiteral("C++%1").arg(__cplusplus >= 202302L ? 23 :
                                     __cplusplus >= 202002L ? 20 :
                                     __cplusplus >= 201703L ? 17 :
                                     __cplusplus >= 201402L ? 14 :
                                     __cplusplus >= 201103L ? 11 : 98);
}

QString
BuildInfo::environment() const
{
#if defined(Q_OS_WIN)
    return QStringLiteral("Windows");
#elif defined(Q_OS_DARWIN)
    return QStringLiteral("Unix/Apple");
#elif defined(Q_OS_BSD4)
    return QStringLiteral("Unix/BSD");
#elif defined(Q_OS_UNIX)
    return QStringLiteral("Unix/Linux");
#else
    return QStringLiteral("Unknown");
#endif
}

QString
BuildInfo::operatingSystem() const
{
    return QSysInfo::prettyProductName();
}

QString
BuildInfo::architecture() const
{
    return QSysInfo::buildCpuArchitecture();
}

QString
BuildInfo::architectureBits() const
{
    return QStringLiteral("%1-bit").arg(sizeof(void *) * CHAR_BIT);
}

QString
BuildInfo::compiler() const
{
#if defined(__clang__)
    return QStringLiteral("Clang %1.%2.%3").arg(__clang_major__).arg(__clang_minor__).arg(__clang_patchlevel__);
#elif defined(__GNUC__)
    return QStringLiteral("GCC %1.%2.%3").arg(__GNUC__).arg(__GNUC_MINOR__).arg(__GNUC_PATCHLEVEL__);
#elif defined(_MSC_VER)
    return QStringLiteral("MSVC %1").arg(_MSC_VER);
#else
    return QStringLiteral("Unknown");
#endif
}

QString
BuildInfo::characterEncoding() const
{
    return QStringLiteral("Unicode");
}

QString
BuildInfo::cLibrary() const
{
#if defined(__GLIBC__)
    return QStringLiteral("GNU glibc %1.%2").arg(__GLIBC__).arg(__GLIBC_MINOR__);
#elif defined(_MSC_VER)
    return QStringLiteral("Microsoft CRT");
#else
    return QStringLiteral("Unknown");
#endif
}

QString
BuildInfo::cppLibrary() const
{
#if defined(_LIBCPP_VERSION)
    return QStringLiteral("libc++ %1").arg(_LIBCPP_VERSION);
#elif defined(__GLIBCXX__)
    return QStringLiteral("GNU libstdc++ %1").arg(__GLIBCXX__);
#elif defined(_MSVC_STL_VERSION)
    return QStringLiteral("Microsoft STL %1").arg(_MSVC_STL_VERSION);
#else
    return QStringLiteral("Unknown");
#endif
}

QString
BuildInfo::qtVersion() const
{
    return QStringLiteral("Qt %1").arg(QString::fromLatin1(qVersion()));
}

} // namespace qtlib
