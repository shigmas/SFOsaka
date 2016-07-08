#include "FJClient.h"

#include "FJOperation.h"
#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include <QThread>
#include <QTimer>

const int FJClient::DefaultPingInterval = 3600;

FJClient::FJClient(const QString& host, const QString& baseUrl,
                   const QString& scheme, QObject *parent) :
    FJClient(scheme + "://" + host + baseUrl, parent)
{
}

FJClient::FJClient(const QUrl& baseUrl, QObject *parent) :
    QObject(parent),
    _url(baseUrl),
    _pingInterval(DefaultPingInterval),
    _accessManager(new QNetworkAccessManager())
{
    qDebug() << "FJClient::FJClient()";
    _StartTimer();
}

FJClient::~FJClient()
{
    qDebug() << "FJClient::~FJClient()";
}

int
FJClient::GetPingInterval() const
{
    return _pingInterval;
}

void
FJClient::SetPingInterval(int seconds)
{
    if (seconds != _pingInterval) {
        _pingInterval = seconds;
        _StartTimer();
    }
}

QUrl
FJClient::GetBaseUrl() const
{
    return _url;
}

void
FJClient::AddOperation(FJOperationSharedPtr operation)
{
    operation->SetClient(this);
    _operationQueue.append(operation);
}

void
FJClient::ClearCookies()
{
    QNetworkCookieJar *cookies = new QNetworkCookieJar();
    _accessManager->setCookieJar(cookies);
}

QByteArray
FJClient::GetCSRFToken() const
{
    return _csrfToken;
}

void
FJClient::SetCSRFToken(const QByteArray& token)
{
    _csrfToken = token;
}

void
FJClient::_StartTimer()
{
    if (_timer) {
        QTimer *oldTimer = _timer.get();
        QObject::disconnect(oldTimer, &QTimer::timeout,
                            this, &FJClient::_HandleTimer);
    }
    QTimer *newTimer = new QTimer();
    QObject::connect(newTimer, &QTimer::timeout,
                     this, &FJClient::_HandleTimer);
    newTimer->start(_pingInterval*1000);

    _timer = QTimerSharedPtr(newTimer);
}

void
FJClient::_HandleTimer()
{
    FJOperationSharedPtr op;
    foreach(op, _operationQueue) {
        if (op) {
            if (op->IsQueued()) {
                //qDebug() << QThread::currentThreadId() << ": Running op";
                op->Execute(_accessManager, _url);
                //qDebug() << QThread::currentThreadId() << ": Op executed";
            } else if (op->IsCompleted()) {
                // Op is completed, remove it from the queue.
                qDebug() << "Op finished. Removing";
                _operationQueue.removeOne(op);
            } else {
            }
        }
    }
}
