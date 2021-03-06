// Based on: https://github.com/RileyGB/BlackBerry10-Samples

#include "webimageview.h"
#include <QNetworkReply>
#include <QNetworkDiskCache>
#include <QtGui/QDesktopServices>
#include <QStringList>
#include <bb/cascades/Image>

#include "cacheserver.h"

using namespace bb::cascades;

QNetworkAccessManager * WebImageView::mNetManager = new QNetworkAccessManager();
QNetworkDiskCache * WebImageView::mNetworkDiskCache = new QNetworkDiskCache();

const QString WebImageView::availableColors[5] = {"green", "blue", "orange", "pink", "grey"};
const QString WebImageView::spriteMap[5][10] = {
        {"plus","home","label-2","star","label","pin","sheet","power","diamond","folder"},
        {"enveloppe","happy-face","rss","calc","clock","pen","bug","label-box","yen","snail"},
        {"cloud","cog","vbar","pie","table","line","magnifier","potion","pound","euro"},
        {"lightbulb","movie","note","camera","mobile","computer","heart","bubbles","dollars"},
        {"alert","bill","funnel","eye","bubble","calendar","check","crown","plane"}
    };

WebImageView::WebImageView() {
    // Initialize network cache
    mNetworkDiskCache->setCacheDirectory(QDesktopServices::storageLocation(QDesktopServices::CacheLocation));

    // Set cache in manager
    mNetManager->setCache(mNetworkDiskCache);

    // Set defaults
    mLoading = 0;
    mIsLoaded = false;
    doSizeCheck = false;
}

const QUrl& WebImageView::url() const {
    return mUrl;
}

bb::ImageData WebImageView::fromQImage(const QImage &qImage)
{
    bb::ImageData imageData(bb::PixelFormat::RGBA_Premultiplied, qImage.width(), qImage.height());

    unsigned char *dstLine = imageData.pixels();
    for (int y = 0; y < imageData.height(); y++) {
        unsigned char * dst = dstLine;
        for (int x = 0; x < imageData.width(); x++) {
            QRgb srcPixel = qImage.pixel(x, y);
            *dst++ = qRed(srcPixel) * qAlpha(srcPixel) / 255;
            *dst++ = qGreen(srcPixel) * qAlpha(srcPixel) / 255;
            *dst++ = qBlue(srcPixel) * qAlpha(srcPixel) / 255;
            *dst++ = qAlpha(srcPixel);
        }
        dstLine += imageData.bytesPerLine();
    }

    return imageData;
}

int WebImageView::getWidth() const
{
    return sourceWidth;
}

/*int WebImageView::getHeight() const
{
    return sourceHeight;
}

int WebImageView::getSize() const
{
    return sourceSize;
}*/

bool WebImageView::getDoSizeCheck()
{
    return doSizeCheck;
}

void WebImageView::setDoSizeCheck(bool value)
{
    if (doSizeCheck != value) {
        doSizeCheck = value;
        emit doSizeCheckChanged();
    }
}

void WebImageView::setUrl(const QUrl& url)
{
    //qDebug() << "url" << url << "mUrl" << mUrl << (url==mUrl);
    if (url == mUrl) {
        return;
    }

    mLoading = 0;
    mIsLoaded = false;

    mUrl = url;
    mLoading = 0;
    mIsLoaded = false;
    emit isLoadedChanged();
    resetImage();

    if (url.isEmpty()) {
        emit urlChanged();
        return;
    }

    // Detecting if url is "asset:///"
    if (url.toString().startsWith("asset:///")) {
        this->setImageSource(url);
        mIsLoaded = true;
        emit isLoadedChanged();
        emit urlChanged();
        return;
    }

    // Detecting if url is "image://nvicons/"
    if (url.toString().startsWith("image://nvicons/")) {
        QStringList parts = url.toString().split('?');
        QString color = parts.at(1);
        parts = parts.at(0).split('/');
        QString icon = parts.at(3);

        setImage(Image(fromQImage(QImage("app/native/assets/sprite-icons.png").copy(getPosition(icon, color)))));
        mIsLoaded = true;
        emit isLoadedChanged();
        emit urlChanged();
        return;
    }

    // Detecting if url is "cache://"
    if (url.toString().startsWith("cache://")) {
        QStringList parts = url.toString().split('/');
        QString filename = parts.at(2);

        Settings *s = Settings::instance();
        DatabaseManager::CacheItem item = s->db->readCacheByEntry(filename);
        if (item.id == "") {
            item = s->db->readCacheByFinalUrl(filename);
        } else {
            filename = item.finalUrl;
        }

        filename = s->getDmCacheDir() + "/" + filename;

        if (!QFile::exists(filename)) {
            emit urlChanged();
            return;
        }

        if (item.contentType == "image/x-icon") {
            // BB does not support ICO image format -> must convert
            QFile file(filename);
            if (!file.open(QIODevice::ReadOnly)) {
                qWarning() << "Could not open" << filename << "for reading: " << file.errorString();
                file.close();
                emit urlChanged();
                return;
            }
            QByteArray data;
            data.append(file.readAll());
            file.close();
            setImage(Image(fromQImage(QImage::fromData(data))));
        } else {
            setImage(Image(QUrl(filename)));
        }

        mIsLoaded = true;
        emit isLoadedChanged();
        emit urlChanged();
        return;
    }

    // Create request
    QNetworkRequest request;
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    request.setUrl(url);

    // Create reply
    QNetworkReply * reply = mNetManager->get(request);

    // Connect to signals
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(imageLoaded()));
    QObject::connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(dowloadProgressed(qint64,qint64)));
    QObject::connect(reply, SIGNAL(metaDataChanged()), this, SLOT(metaDataChanged()));

    emit urlChanged();
}

void WebImageView::metaDataChanged()
{
    QNetworkReply * reply = qobject_cast<QNetworkReply*>(sender());
    //qDebug() << "metaDataChanged:" << reply->url();

    // Memory protection fix -> not loading big images
    if (reply->header(QNetworkRequest::ContentLengthHeader).isValid()) {
        int length = reply->header(QNetworkRequest::ContentLengthHeader).toInt();
        if (length > maxSourceSize) {
            //qDebug() << "metaDataChanged, length=" << length;
            reply->close();
            return;
        }
    }
}

double WebImageView::loading() const
{
    return mLoading;
}

void WebImageView::imageLoaded() {
    // Get reply
    QNetworkReply * reply = qobject_cast<QNetworkReply*>(sender());

    //qDebug() << "error" << reply->error();
    //qDebug() << "imageLoaded:" << reply->url() << reply->error();

    if (reply->error() == QNetworkReply::NoError) {
        if (isARedirectedUrl(reply)) {
            setURLToRedirectedUrl(reply);
            return;
        } else {
            QByteArray imageData = reply->readAll();
            //qDebug() << "imageData.length" << imageData.length();
            // Memory protection & Tiny image fix -> not loading big or tiny images
            if (doSizeCheck &&
                    (imageData.length() > maxSourceSize ||
                imageData.length() < minSourceSize)) {
                mIsLoaded = false;
            } else {
                QImage img = QImage::fromData(imageData);
                int width = img.width();
                //int height = img.height();
                //int size = imageData.length();
                if (width != sourceWidth) {
                    sourceWidth = width;
                    emit widthChanged();
                }
                /*if (height != sourceHeight) {
                    sourceHeight = height;
                    emit heightChanged();
                }
                if (size != sourceSize) {
                    sourceSize = size;
                    emit sizeChanged();
                }*/
                //qDebug() << "ContentType:" << reply->header(QNetworkRequest::ContentTypeHeader).toString();
                if (reply->header(QNetworkRequest::ContentTypeHeader).toString() == "image/x-icon") {
                    // BB does not support ICO image format -> must convert
                    setImage(Image(fromQImage(QImage::fromData(imageData))));
                } else {
                    setImage(Image(imageData));
                }

                mIsLoaded = true;
            }
        }
    } else {
        mIsLoaded = false;
        mUrl.clear();
    }

    emit isLoadedChanged();

    // Memory management
    reply->deleteLater();
}

bool WebImageView::isARedirectedUrl(QNetworkReply *reply) {
    QUrl redirection = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    return !redirection.isEmpty();
}

bool WebImageView::isLoaded() const {
    return mIsLoaded;
}

void WebImageView::setURLToRedirectedUrl(QNetworkReply *reply) {
    QUrl redirectionUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    QUrl baseUrl = reply->url();
    QUrl resolvedUrl = baseUrl.resolved(redirectionUrl);

    setUrl(resolvedUrl.toString());
}

void WebImageView::clearCache() {
    mNetworkDiskCache->clear();
}

void WebImageView::dowloadProgressed(qint64 bytes, qint64 total) {
    mLoading = double(bytes) / double(total);

    emit loadingChanged();
}

QRect WebImageView::getPosition(const QString &icon, const QString &color)
{
    int n = 16, s = 20, a = 16;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (spriteMap[i][j] == icon) {
                n += 100 * i;
                a += j * s;
                return QRect(n + getOffsetByColor(color), a, 16, 16);
            }
        }
    }
    qWarning() << "getPosition failed!";
    return QRect(0,0,0,0);
}

int WebImageView::getOffsetByColor(const QString &color)
{
    int index = 0;
    for (int i = 0; i < 5; ++i) {
        if (availableColors[i] == color) {
            index = i;
            break;
        }
    }
    return index * 20;
}
