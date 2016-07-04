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

public:
    // checkFetch and fetchData are the tail end URL's. Execute gets the base
    // URL
    explicit FJOperation(const QString& name, const QString& checkFetch,
                         const QString& fetchData,
                         FJCaller *caller, QObject *parent = 0);
    virtual ~FJOperation();

    void SetClient(FJClient *client);

    QString GetName() const;
    void SetName(const QString& name);

    void SetCheckFetchContent(const QJsonDocument& checkFetchContent);
    void SetFetchDataContent(const QJsonDocument& fetchDataContent);

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

protected:
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
    QJsonDocument _checkFetchContent;
    QJsonDocument _fetchDataContent;

    bool _isCompleted;

    FJCaller *_caller;
    FJClient *_client;

    QNetworkRequest *_request;
    QNetworkReply *_reply;

    QByteArray _buffer;
    qint64 _bufferSize;
};

#endif // FJOPERATION_H
