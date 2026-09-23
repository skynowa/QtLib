/**
 * \file  BuildInfo.h
 * \brief Build and runtime information for the About dialog
 */

#pragma once

#include <QString>

namespace qtlib
{

class BuildInfo final
{
public:
    QString configuration() const;
    QString dateTime() const;
    QString languageStandard() const;
    QString environment() const;
    QString operatingSystem() const;
    QString architecture() const;
    QString architectureBits() const;
    QString compiler() const;
    QString characterEncoding() const;
    QString cLibrary() const;
    QString cppLibrary() const;
    QString qtVersion() const;
};

} // namespace qtlib
