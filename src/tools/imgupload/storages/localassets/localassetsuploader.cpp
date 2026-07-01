// SPDX-License-Identifier: GPL-3.0-or-later
// Based on ImgurUploader, adapted for self-hosted upload server

#include "localassetsuploader.h"
#include "utils/confighandler.h"
#include "utils/filenamehandler.h"
#include "utils/history.h"
#include "widgets/loadspinner.h"
#include "widgets/notificationwidget.h"

#include <QBuffer>
#include <QDateTime>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QShortcut>
#include <QUrl>

// ---------------------------------------------------------------------
// CONFIGURE THIS: your self-hosted upload endpoint
// ---------------------------------------------------------------------
static const char* UPLOAD_ENDPOINT = "https://localassets.letmegrab.in/ss";

LocalAssetsUploader::LocalAssetsUploader(const QPixmap& capture,
                                          QWidget* parent)
  : ImgUploaderBase(capture, parent)
{
    m_NetworkAM = new QNetworkAccessManager(this);
    connect(m_NetworkAM,
            &QNetworkAccessManager::finished,
            this,
            &LocalAssetsUploader::handleReply);
}

void LocalAssetsUploader::handleReply(QNetworkReply* reply)
{
    spinner()->deleteLater();
    m_currentImageName.clear();

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray raw = reply->readAll();
        QJsonDocument response = QJsonDocument::fromJson(raw);

        QString finalUrl;
        if (response.isObject()) {
            // Adjust this key to match your server's JSON response,
            // e.g. {"url": "https://localassets.letmegrab.in/f/xyz.png"}
            QJsonObject json = response.object();
            finalUrl = json.value(QStringLiteral("url")).toString();
        } else {
            // Fallback: server returned a plain-text URL body
            finalUrl = QString::fromUtf8(raw).trimmed();
        }

        if (finalUrl.isEmpty()) {
            setInfoLabelText(tr("Upload succeeded but no URL was returned "
                                 "by the server."));
            new QShortcut(Qt::Key_Escape, this, SLOT(close()));
            return;
        }

        setImageURL(QUrl(finalUrl));

        m_currentImageName = imageURL().toString();
        int lastSlash = m_currentImageName.lastIndexOf("/");
        if (lastSlash >= 0) {
            m_currentImageName = m_currentImageName.mid(lastSlash + 1);
        }

        // No delete-token concept on a plain self-hosted server;
        // store an empty token. Adjust if your server supports delete.
        History history;
        m_currentImageName =
          history.packFileName("localassets", "", m_currentImageName);
        history.save(pixmap(), m_currentImageName);

        emit uploadOk(imageURL());
    } else {
        setInfoLabelText(reply->errorString());
    }
    new QShortcut(Qt::Key_Escape, this, SLOT(close()));
}

void LocalAssetsUploader::upload()
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    pixmap().save(&buffer, "PNG");

    QString fileName =
      QStringLiteral("screenshot_%1.png")
        .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));

    auto* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentTypeHeader,
                        QVariant("image/png"));
    filePart.setHeader(
      QNetworkRequest::ContentDispositionHeader,
      QVariant(QStringLiteral("form-data; name=\"file\"; filename=\"%1\"")
                 .arg(fileName)));
    filePart.setBody(byteArray);
    multiPart->append(filePart);

    QUrl url(QString::fromUtf8(UPLOAD_ENDPOINT));
    QNetworkRequest request(url);

    // If your on-prem server requires auth (API key / bearer token),
    // set it here, e.g.:
    // request.setRawHeader("Authorization",
    //     QStringLiteral("Bearer %1").arg(ConfigHandler().uploadClientSecret()).toUtf8());

    QNetworkReply* reply = m_NetworkAM->post(request, multiPart);
    multiPart->setParent(reply);
}

void LocalAssetsUploader::deleteImage(const QString& fileName,
                                       const QString& deleteToken)
{
    Q_UNUSED(fileName)
    Q_UNUSED(deleteToken)
    // No hosted delete endpoint by default for a self-hosted server.
    // Implement a DELETE request to your server here if you add that route.
    notification()->showMessage(
      tr("Delete is not configured for this server."));
    emit deleteOk();
}
