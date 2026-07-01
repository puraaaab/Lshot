// SPDX-License-Identifier: GPL-3.0-or-later
// Based on ImgurUploader, adapted for self-hosted upload server

#pragma once

#include "tools/imgupload/storages/imguploaderbase.h"

#include <QUrl>
#include <QWidget>

class QNetworkReply;
class QNetworkAccessManager;
class QUrl;

class LocalAssetsUploader : public ImgUploaderBase
{
    Q_OBJECT
public:
    explicit LocalAssetsUploader(const QPixmap& capture,
                                  QWidget* parent = nullptr);
    void deleteImage(const QString& fileName, const QString& deleteToken);

private slots:
    void handleReply(QNetworkReply* reply);

private:
    void upload();

private:
    QNetworkAccessManager* m_NetworkAM;
};
