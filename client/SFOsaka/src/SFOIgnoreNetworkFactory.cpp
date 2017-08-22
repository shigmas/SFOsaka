#include "SFOIgnoreNetworkFactory.h"

#include "SFOIgnoreNetworkAccessManager.h"

QNetworkAccessManager*
SFOIgnoreNetworkFactory::create(QObject *parent)
{
    return new SFOIgnoreNetworkAccessManager(parent);
}
