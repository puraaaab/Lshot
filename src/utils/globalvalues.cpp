// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2017-2019 Alejandro Sirgo Rica & Contributors

#include "globalvalues.h"

#include <QApplication>
#include <QFontMetrics>
#if defined(Q_OS_MACOS)
#include <QOperatingSystemVersion>
#endif

int GlobalValues::buttonBaseSize()
{
    return QFontMetrics(qApp->font()).lineSpacing() * 2.2;
}

QString GlobalValues::versionInfo()
{
    return QStringLiteral("Lshot " APP_VERSION " (" FLAMESHOT_GIT_HASH ")"
                          "\nCompiled with Qt " QT_VERSION_STR);
}

QString GlobalValues::iconPath()
{
#if USE_MONOCHROME_ICON
    return QString(":img/app/lshot.monochrome.svg");
#else
    return { ":img/app/lshot.svg" };
#endif
}

QString GlobalValues::iconPathPNG()
{
#if USE_MONOCHROME_ICON
    return QString(":img/app/lshot.monochrome.png");
#else
    return { ":img/app/lshot.png" };
#endif
}

QString GlobalValues::trayIconPath()
{
#if USE_MONOCHROME_ICON
#if defined(Q_OS_MACOS)
    auto currentMacOsVersion = QOperatingSystemVersion::current();
    if (currentMacOsVersion >= QOperatingSystemVersion::MacOSBigSur) {
        return { ":img/app/lshot.mask.png" };
    } else {
        return { ":img/app/lshot.monochrome.png" };
    }
#else
    return { ":img/app/lshot.monochrome.png" };
#endif
#else
    return { ":img/app/lshot.png" };
#endif
}
