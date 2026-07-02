// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2017-2019 Alejandro Sirgo Rica & Contributors

#include "CapShotdbusadapter.h"
#include "core/CapShot.h"
#include "core/CapShotdaemon.h"

CapShotDBusAdapter::CapShotDBusAdapter(QObject* parent)
  : QDBusAbstractAdaptor(parent)
{}

CapShotDBusAdapter::~CapShotDBusAdapter() = default;

void CapShotDBusAdapter::captureScreen()
{
    CapShot::instance()->gui(CaptureRequest(CaptureRequest::GRAPHICAL_MODE));
}

void CapShotDBusAdapter::attachScreenshotToClipboard(const QByteArray& data)
{
    CapShotDaemon::instance()->attachScreenshotToClipboard(data);
}

void CapShotDBusAdapter::attachTextToClipboard(const QString& text,
                                                 const QString& notification)
{
    CapShotDaemon::instance()->attachTextToClipboard(text, notification);
}

void CapShotDBusAdapter::attachPin(const QByteArray& data)
{
    CapShotDaemon::instance()->attachPin(data);
}
