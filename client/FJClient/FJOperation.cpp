#include "FJOperation.h"

#include "FJCaller.h"
#include "FJClient.h"

#include <QAuthenticator>
#include <QDebug>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QIODevice>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QThread>
#include <QVariant>

#include <iostream>
#include <functional>

using namespace std::placeholders;

const QByteArray FJOperation::CSRFCookieName = "csrftoken";
const QByteArray FJOperation::CSRFHeaderName = "X-CSRFTOKEN";


FJOperation::FJOperation(const QString& name, const QString& checkFetch,
                         const QString& fetchData,
                         FJCaller *caller, QObject *parent) :
    QObject(parent), _name(name), _checkFetchUrl(checkFetch),
    _fetchDataUrl(fetchData), _isPost(true), _status(FJQueuedStatus),
    _caller(caller), _request(NULL)
{
}

FJOperation::~FJOperation()
{
    qDebug() << "[" << _name << "] FJOperation::~FJOperation()";
}

void
FJOperation::SetClient(FJClient *client)
{
    _client = client;
}

void
FJOperation::SetIsPost(bool isPost)
{
    _isPost = isPost;
}

QString
FJOperation::GetName() const
{
    return _name;
}

void
FJOperation::SetName(const QString& name)
{
    if (_name != name) {
        _name = name;
    }
}

void
FJOperation::SetCheckFetchContent(const QJsonDocument& checkFetchContent)
{
    _checkFetchContent = checkFetchContent;
}

void
FJOperation::SetFetchDataContent(const QJsonDocument& fetchDataContent)
{
    _fetchDataContent = fetchDataContent;
}

bool
FJOperation::IsQueued() const
{
    return _status == FJQueuedStatus;
}

bool
FJOperation::IsInProcess() const
{
    return _status == FJInProcessStatus;
}

bool
FJOperation::IsCompleted() const
{
    return _status == FJCompletedStatus;
}

void
FJOperation::Execute(QNetworkAccessManagerPtr accessManager,
                     const QUrl& baseUrl)
{
    if (!_checkFetchUrl.isEmpty()) {
        _RunRequest(accessManager, baseUrl, FJCheckOperationType);
    } else {
        _RunRequest(accessManager, baseUrl, FJFetchOperationType);
    }
}

void
FJOperation::_OnAuthenticationRequired(QNetworkReply *,
                                       QAuthenticator * authenticator)
{
    std::cerr << "Authentication required. authenticating..." << std::endl;
    authenticator->setUser("guest");
    authenticator->setPassword("guest");
    // Send the request again
    //SendRequest(_request->url().toString());
}

void
_NetworkErrorPrintError(const QNetworkReply::NetworkError& error)
{
    switch(error) {
    case QNetworkReply::NoError:
        qDebug() << "No Error";
        break;
    default:
        qDebug() << "Unknown Error: " << error;
    }
}

void
FJOperation::_OnReplyFinished(QNetworkAccessManagerSharedPtr accessManager,
                              const QUrl& baseUrl,
                              FJOperationType opType)
{
    QObject::disconnect(_reply, 0, 0, 0);
    if (_reply->error() == QNetworkReply::ProtocolUnknownError) {
        // Start a new request
        QVariant locationHeader = _reply->header(QNetworkRequest::LocationHeader);
        if (locationHeader.type() != QVariant::String) {
            exit(-1);
        }
        QString locationString = locationHeader.toString();
        _RunRequest(accessManager, baseUrl, opType);
    } else {
        _StoreCSRF(accessManager, baseUrl);
        QJsonDocument jsonDoc = _GetJsonFromContent(_buffer);
        if (jsonDoc.isNull() || jsonDoc.isEmpty()) {
            _NetworkErrorPrintError(_reply->error());
            qDebug() << "Invalid JSON for " << baseUrl << ": " << _buffer;
        } else if (opType == FJCheckOperationType) {
            if (_ProcessCheck(jsonDoc)) {
                _RunRequest(accessManager, baseUrl, FJFetchOperationType);
            } else {
                qDebug() << "No pull needed";
                _status = FJCompletedStatus;
            }
        } else if (opType == FJFetchOperationType) {
            qDebug() << "Data pull finished";
            if (_caller) {
                _caller->HandleResponse(jsonDoc, FJNoError, this);
            }
            _status = FJCompletedStatus;
        }
    }
    qDebug() << "_OnReplyfinished completed";
}

void
FJOperation::_OnBytesReceived(qint64 bytesReceived, qint64 )
{
    if (bytesReceived <= 0) {
        std::cerr << "Nothing to read" << std::endl;
        return;
    }
    char buffer[bytesReceived+1];
    qint64 bytesRead = _reply->read(buffer, bytesReceived);
    if (bytesRead != bytesReceived) {
        qDebug() << "Read " << bytesRead << ", expected " << bytesReceived;
    }
    _buffer.append(buffer);
}

void
FJOperation::_OnReadyRead()
{
    qDebug() << "Operation: OnReadyRead";
    // We hack a little here, since this is a GET handler, which is only used for
    // single requests (not the check, then fetch), so we assume that the caller
    // wants to get this request and there isn't a fetch coming.
    qDebug() << "ReadyRead content: " << _buffer;
    _buffer.append(_reply->readAll());
}

void
FJOperation::_OnFinished(QNetworkAccessManagerSharedPtr accessManager,
                         const QUrl& baseUrl)
{
    qDebug() << "Operation: OnFinished";
    QObject::disconnect(_reply, 0, 0, 0);
    _StoreCSRF(accessManager, baseUrl);
    _buffer.append(_reply->readAll());
    QJsonDocument jsonDoc = _GetJsonFromContent(_buffer);
    if (!jsonDoc.isNull() and !jsonDoc.isEmpty()) {
        if (_caller) {
            qDebug() << "Response content: " << jsonDoc;
            _caller->HandleResponse(jsonDoc, FJNoError, this);
        }
    } else {
        _NetworkErrorPrintError(_reply->error());
        qDebug() << "Invalid or empty JSON in response";
    }
    qDebug() << "_OnFinished completed";
    _status = FJCompletedStatus;
}

void
FJOperation::_OnSslErrors(const QList<QSslError>& errors)
{
    qDebug() << "Got SSL Errors: " << errors;
    // XXX - Hacky hack hack. Android fails on handshake error. This let's us
    // get by that. Should really do a better review on this. (We are using
    // a signed cert, but apparently, not trusted enough for android.)
    _reply->ignoreSslErrors(errors);
}

void
FJOperation::_StoreCSRF(const QNetworkAccessManagerSharedPtr& accessManager,
                        const QUrl& baseUrl)
{
    QNetworkCookie cookie;
    foreach(cookie, accessManager->cookieJar()->cookiesForUrl(baseUrl)) {
        if (cookie.name() == CSRFCookieName) {
            if (_client) {
                _client->SetCSRFToken(cookie.value());
            }
        }
    }
}

QJsonDocument
FJOperation::_GetJsonFromContent(const QByteArray& content)
{
    if (content.size() <= 0) {
        qDebug() << "Buffer empty. returning";
        return QJsonDocument();
    }

    QByteArray cleanedContent = content;
    unsigned int desiredSize = cleanedContent.size() -1;
    //qDebug() << "Raw: <<" << _buffer.data() << ">>";
    while ((cleanedContent.at(desiredSize) != '}') &&
           (cleanedContent.at(desiredSize) != ']')) {
        // Remove crap at the end. Namely, the ';' that the server sticks on.
        // That's never valid JSON, so F-U rabbitmq management plugin.
        cleanedContent.chop(1);
        desiredSize--;
    }
    //qDebug() << "Trunc: <<" << _buffer.data() << ">>";
    return QJsonDocument::fromJson(cleanedContent);
}

void
FJOperation::_RunRequest(QNetworkAccessManagerPtr accessManagerPtr,
                         const QUrl& baseUrl,
                         FJOperationType opType)
{
    qDebug() << "_RunRequest() - " << _name;
    _status = FJInProcessStatus;
    auto accessManager = accessManagerPtr.lock();
    if (!accessManager) {
        qDebug() << "No access manager. Skipping";
        return;
    }
    qDebug() << "_RunRequest() - access: " << accessManager->networkAccessible();
    QObject::connect(accessManager.get(),
                     &QNetworkAccessManager::authenticationRequired,
                     this,
                     &FJOperation::_OnAuthenticationRequired);

    QUrl requestUrl = baseUrl;
    QString urlPath = requestUrl.path();
    QJsonDocument *jsonDoc = NULL;
    
    if (!urlPath.endsWith("/")) {
        urlPath.append("/");
    }
    if (opType == FJCheckOperationType) {
        urlPath.append(_checkFetchUrl);
        jsonDoc = &_checkFetchContent;
    } else {
        urlPath.append(_fetchDataUrl);
        jsonDoc = &_fetchDataContent;
    }
    urlPath = urlPath.replace("//","/");
    requestUrl.setPath(urlPath);

    //    if (_request) {
    //        std::cerr << "Deleting old request" << std::endl;
    //        delete _request;
    //        _request = NULL;
    //    }
    _request = new QNetworkRequest(requestUrl);
    QByteArray jsonContent;
    if (_isPost) {
        if (!jsonDoc->isEmpty() || !jsonDoc->isNull()) {
            jsonContent = jsonDoc->toJson(); 
        }
        _request->setRawHeader(CSRFHeaderName, _client->GetCSRFToken());
        _request->setHeader(QNetworkRequest::ContentTypeHeader,
                            "application/json");
        _reply = accessManager->post(*_request,jsonContent);
        QObject::connect(_reply, &QNetworkReply::finished,
                         std::bind(&FJOperation::_OnReplyFinished, this,
                                   accessManager, baseUrl, opType));
        QObject::connect(_reply, &QNetworkReply::downloadProgress,
                         this, &FJOperation::_OnBytesReceived);
    } else {
        _reply = accessManager->get(*_request);
        QObject::connect(_reply, &QNetworkReply::readyRead,
                         this, &FJOperation::_OnReadyRead);
        QObject::connect(_reply, &QNetworkReply::finished,
                         std::bind(&FJOperation::_OnFinished, this,
                                   accessManager, baseUrl));
        QObject::connect(_reply, &QNetworkReply::sslErrors,
                         this, &FJOperation::_OnSslErrors);
    }
    _buffer.clear();
}

bool
FJOperation::_ProcessCheck(const QJsonDocument& document) 
{
    if (! document.isObject()) {
        std::cerr << "Expected document to be an object" << std::endl;
    }

    QVariantMap jMap = document.object().toVariantMap();
    bool result = FJType_GetValueFromMap("result",jMap, false);
    if (!result) {
        return false;
    }
    return FJType_GetValueFromMap("update_needed",jMap, false);
    
}
