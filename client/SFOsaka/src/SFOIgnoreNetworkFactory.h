#ifndef SFOIGNORENETWORKFACTORY_H
#define SFOIGNORENETWORKFACTORY_H

#include <QQmlNetworkAccessManagerFactory>

class SFOIgnoreNetworkFactory: public QObject, public QQmlNetworkAccessManagerFactory
{
    Q_OBJECT

public:
    virtual QNetworkAccessManager *create(QObject *parent);
};
#endif
