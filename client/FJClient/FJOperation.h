#ifndef FJOPERATION_H
#define FJOPERATION_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>

#include "FJTypes.h"

FJ_DECLARE_PTRS(FJCaller)
FJ_DECLARE_PTRS(FJClient)
FJ_DECLARE_PTRS(QNetworkAccessManager)

class QAuthenticator;
class QNetworkReply;
class QNetworkRequest;

class FJOperation : public QObject
{
    Q_OBJECT

    enum FJOperationType {
        FJCheckOperationType,
        FJFetchOperationType,
    };

    enum FJOperationStatus {
        FJQueuedStatus,
        FJInProcessStatus,
        FJCompletedStatus,
    };

public:
    // checkFetch and fetchData are the tail end URL's. Execute gets the base
    // URL
    explicit FJOperation(const QString& name, const QString& checkFetch,
                         const QString& fetchData,
                         FJCaller *caller, QObject *parent = 0);
    virtual ~FJOperation();

    void SetClient(FJClient *client);
    
    void SetIsPost(bool isPost);

    QString GetName() const;
    void SetName(const QString& name);

    void SetCheckFetchContent(const QJsonDocument& checkFetchContent);
    void SetFetchDataContent(const QJsonDocument& fetchDataContent);

    bool IsQueued() const;
    bool IsInProcess() const;
    bool IsCompleted() const;

    // Run the operation. If a fetch is needed, HandleResponse in the 
    void Execute(QNetworkAccessManagerPtr accessManager, const QUrl& baseUrl);

signals:

protected slots:
    void _OnAuthenticationRequired(QNetworkReply * reply,
                                   QAuthenticator * authenticator);
    virtual void _OnReplyFinished(QNetworkAccessManagerSharedPtr accessManager,
                                  const QUrl& baseUrl,
                                  FJOperationType opType);
    virtual void _OnBytesReceived(qint64 bytesReceived, qint64 bytesTotal);
    virtual void _OnReadyRead();
    virtual void _OnFinished(QNetworkAccessManagerSharedPtr accessManager,
                             const QUrl& baseUrl);

protected:
    void _StoreCSRF(const QNetworkAccessManagerSharedPtr& accessManager,
                    const QUrl& baseUrl);

    QJsonDocument _GetJsonFromContent(const QByteArray& content);

    void _RunRequest(QNetworkAccessManagerPtr accessManager,
                     const QUrl& baseUrl,
                     FJOperationType opType);

    bool _ProcessCheck(const QJsonDocument& document);

private:
    static const QByteArray CSRFCookieName;
    static const QByteArray CSRFHeaderName;

    QString _name;
    QString _checkFetchUrl;
    QString _fetchDataUrl;
    bool _isPost;
    QJsonDocument _checkFetchContent;
    QJsonDocument _fetchDataContent;

    FJOperationStatus _status;

    FJCaller *_caller;
    FJClient *_client;

    QNetworkRequest *_request;
    QNetworkReply *_reply;

    QByteArray _buffer;
    qint64 _bufferSize;
};

#endif // FJOPERATION_H
