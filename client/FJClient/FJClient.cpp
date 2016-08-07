#include "FJClient.h"

#include "FJOperation.h"
#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include <QThread>
#include <QTimer>

// Ping once an hour
const int FJClient::DefaultPingInterval = 3600000;
// But, check every half second when we're flushing the queue. 
const int FJClient::DefaultFlushInterval = 500;

FJClient::FJClient(const QString& host, const QString& baseUrl,
                   const QString& scheme, QObject *parent) :
    FJClient(scheme + "://" + host + baseUrl, parent)
{
}

FJClient::FJClient(const QUrl& baseUrl, QObject *parent) :
    QObject(parent),
    _url(baseUrl),
    _pingInterval(DefaultPingInterval),
    _inQueueFlush(false),
    _accessManager(new QNetworkAccessManager())
{
    _StartTimer(_pingInterval);
}

FJClient::~FJClient()
{
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
        _StartTimer(_pingInterval);
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
FJClient::FlushQueue()
{
    _inQueueFlush = true;
    _StartTimer(DefaultFlushInterval);
}

bool
FJClient::IsNetworkAccessible() const
{
    return _accessManager->networkAccessible() ==
        QNetworkAccessManager::Accessible;
}

void
FJClient::_StartTimer(int timerInterval)
{
    if (_timer) {
        QTimer *oldTimer = _timer.get();
        QObject::disconnect(oldTimer, &QTimer::timeout,
                            this, &FJClient::_HandleTimer);
    }
    QTimer *newTimer = new QTimer();
    QObject::connect(newTimer, &QTimer::timeout,
                     this, &FJClient::_HandleTimer);
    newTimer->start(timerInterval);

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
                //qDebug() << "Op finished. Removing";
                _operationQueue.removeOne(op);
            }
        }
    }

    if (_operationQueue.empty()) {
        if (_inQueueFlush) {
            _inQueueFlush = false;
            // Flushed all ops. Go back to regular ping interval
            _StartTimer(_pingInterval);
        }
        emit QueueCompleted();
    }
}
