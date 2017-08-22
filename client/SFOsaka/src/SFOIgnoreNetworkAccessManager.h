#ifndef SFOIGNORENETWORKACCESSMANAGER_H
#define SFOIGNORENETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>

class SFOIgnoreNetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT

public:
    SFOIgnoreNetworkAccessManager(QObject *parent = 0);

public slots:
    void ignoreSSLErrors(QNetworkReply *reply, const QList<QSslError>& errors);
};


#endif
