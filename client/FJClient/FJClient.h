#ifndef FJCLIENT_H
#define FJCLIENT_H

#include <QList>
#include <QObject>
#include <QString>
#include <QUrl>

#include "FJTypes.h"

FJ_DECLARE_PTRS(FJOperation)
FJ_DECLARE_PTRS(QNetworkAccessManager)
FJ_DECLARE_PTRS(QTimer)

class FJClient : public QObject
{
    Q_OBJECT

public:
    FJClient(const QString& host, const QString& baseUrl,
             const QString& scheme = "https", QObject *parent = 0);
    FJClient(const QUrl& baseUrl, QObject *parent = 0);

    virtual ~FJClient();
    int GetPingInterval() const;

    void SetPingInterval(int seconds);

    QUrl GetBaseUrl() const;

    void AddOperation(FJOperationSharedPtr operation);

    void ClearCookies();

    QByteArray GetCSRFToken() const;

    void SetCSRFToken(const QByteArray& token);

    // Runs the queue until it's empty or a new ping interval is set
    void FlushQueue();

    bool IsNetworkAccessible() const;

    static const int DefaultPingInterval;
    static const int DefaultFlushInterval;

signals:
    void QueueCompleted();

protected:
    void _StartTimer(int pingInterval);

protected slots:
    void _HandleTimer();

private:
    QUrl _url;
    QList<FJOperationSharedPtr> _operationQueue;
    int _pingInterval;

    bool _inQueueFlush;

    QNetworkAccessManagerSharedPtr _accessManager;
    QTimerSharedPtr _timer;
    QByteArray _csrfToken;
};

#endif // FJCLIENT_H
