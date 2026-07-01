// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2017-2019 Alejandro Sirgo Rica & Contributors

#include "lshotdbusadapter.h"
#include "core/lshot.h"
#include "core/lshotdaemon.h"

LshotDBusAdapter::LshotDBusAdapter(QObject* parent)
  : QDBusAbstractAdaptor(parent)
{}

LshotDBusAdapter::~LshotDBusAdapter() = default;

void LshotDBusAdapter::captureScreen()
{
    Lshot::instance()->gui(CaptureRequest(CaptureRequest::GRAPHICAL_MODE));
}

void LshotDBusAdapter::attachScreenshotToClipboard(const QByteArray& data)
{
    LshotDaemon::instance()->attachScreenshotToClipboard(data);
}

void LshotDBusAdapter::attachTextToClipboard(const QString& text,
                                                 const QString& notification)
{
    LshotDaemon::instance()->attachTextToClipboard(text, notification);
}

void LshotDBusAdapter::attachPin(const QByteArray& data)
{
    LshotDaemon::instance()->attachPin(data);
}
