// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2017-2019 Alejandro Sirgo Rica & Contributors

#include "pathinfo.h"

#include <QApplication>
#include <QDir>
#include <QFileInfo>

const QString PathInfo::whiteIconPath()
{
    return QStringLiteral(":/img/material/white/");
}

const QString PathInfo::blackIconPath()
{
    return QStringLiteral(":/img/material/black/");
}

QStringList PathInfo::translationsPaths()
{
    QString binaryPath =
      QFileInfo(qApp->applicationDirPath()).absoluteFilePath();
    QString trPath = QDir::toNativeSeparators(binaryPath + "/translations");
#if defined(Q_OS_UNIX)
    return QStringList()
           << QStringLiteral(APP_PREFIX) + "/share/CapShot/translations"
           << trPath << QStringLiteral("/usr/share/CapShot/translations")
           << QStringLiteral("/usr/local/share/CapShot/translations");
#endif
    return QStringList() << trPath;
}
