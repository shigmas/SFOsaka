#include "SFOIgnoreNetworkAccessManager.h"

#include <QDebug>
#include <QNetworkReply>

SFOIgnoreNetworkAccessManager::SFOIgnoreNetworkAccessManager(QObject *parent) :
    QNetworkAccessManager(parent)
{
    QObject::connect(this,SIGNAL(sslErrors(QNetworkReply*,
                                           const QList<QSslError>&)),
                     this,SLOT(ignoreSSLErrors(QNetworkReply*,
                                               const QList<QSslError>&)));
}

void
SFOIgnoreNetworkAccessManager::ignoreSSLErrors(QNetworkReply* reply,
                                               const QList<QSslError>& errors)
{
    qDebug() << "Ignoring errors";
    reply->ignoreSslErrors(errors);
}
