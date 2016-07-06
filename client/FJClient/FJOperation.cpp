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
    _fetchDataUrl(fetchData), _isCompleted(false), _caller(caller),
    _request(NULL)
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
FJOperation::IsCompleted() const
{
    return _isCompleted;
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
FJOperation::_OnReplyFinished(QNetworkAccessManagerSharedPtr accessManager,
                              const QUrl& baseUrl,
                              FJOperationType opType)
{
    _isCompleted = true;
    if (_reply->error() == QNetworkReply::ProtocolUnknownError) {
        // Start a new request
        QVariant locationHeader = _reply->header(QNetworkRequest::LocationHeader);
        if (locationHeader.type() != QVariant::String) {
            exit(-1);
        }
        QString locationString = locationHeader.toString();
        _RunRequest(accessManager, baseUrl, opType);
    } else {
        QNetworkCookie cookie;
        foreach(cookie, accessManager->cookieJar()->cookiesForUrl(baseUrl)) {
            if (cookie.name() == CSRFCookieName) {
                if (_client) {
                    _client->SetCSRFToken(cookie.value());
                }
            }
        }
        if (_buffer.size() <= 0) {
            return;
        }
        unsigned int desiredSize = _buffer.size() -1;
        //qDebug() << "Raw: <<" << _buffer.data() << ">>";
        while ((_buffer.at(desiredSize) != '}') &&
               (_buffer.at(desiredSize) != ']')) {
            // Remove crap at the end. Namely, the ';' that the server sticks on.
            // That's never valid JSON, so F-U rabbitmq management plugin.
            _buffer.chop(1);
            desiredSize--;
        }
        //qDebug() << "Trunc: <<" << _buffer.data() << ">>";
        QJsonDocument jsonDoc = QJsonDocument::fromJson(_buffer);
        if (jsonDoc.isNull() || jsonDoc.isEmpty()) {
            qDebug() << "Invalid JSON for " << baseUrl << ": " << _buffer;
        } else if (opType == FJCheckOperationType) {
            if (_ProcessCheck(jsonDoc)) {
                _RunRequest(accessManager, baseUrl, FJFetchOperationType);
            }
        } else if (opType == FJFetchOperationType) {
            if (_caller) {
                _caller->HandleResponse(jsonDoc, FJNoError, this);
            }

        }
    }
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
    //std::cerr << bytesReceived << " bytes read" << std::endl;
}



void
FJOperation::_RunRequest(QNetworkAccessManagerPtr accessManagerPtr,
                         const QUrl& baseUrl,
                         FJOperationType opType)
{
//    qDebug() << "_RunRequest() - " << opType;
    auto accessManager = accessManagerPtr.lock();
    if (!accessManager)
        return;

    QObject::connect(accessManager.get(),
                     SIGNAL(authenticationRequired(QNetworkReply*,
                                                   QAuthenticator*)),
                     this,
                     SLOT(_OnAuthenticationRequired(QNetworkReply*,
                                                    QAuthenticator*)));
    
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
    if (_client) {
        _request->setRawHeader(CSRFHeaderName, _client->GetCSRFToken());
    } else {
        qDebug() << "client is null. can't get the token";
    }
    QByteArray jsonContent;
    if (!jsonDoc->isEmpty() || !jsonDoc->isNull()) {
        jsonContent = jsonDoc->toJson(); 
        _request->setHeader(QNetworkRequest::ContentTypeHeader,
                            "application/json");
        _reply = accessManager->post(*_request,jsonContent);
    } else {
        _reply = accessManager->get(*_request);
    }
    _buffer.clear();
    connect(_reply, &QNetworkReply::finished,
            std::bind(&FJOperation::_OnReplyFinished, this,
                      accessManager, baseUrl, opType));
    QObject::connect(_reply, SIGNAL(downloadProgress(qint64,qint64)),
                     this, SLOT(_OnBytesReceived(qint64,qint64)));
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
