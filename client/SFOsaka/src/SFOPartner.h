#ifndef SFOPARTNER_H
#define SFOPARTNER_H

#include "SFOOrganization.h"

// Everything else include in SFOOrganization.h
#include <QGeoLocation>
#include <QList>

class SFOPartner : public SFOOrganization
{
    Q_OBJECT

public:
    explicit SFOPartner(QObject *parent = 0);

    SFOPartner(const QJsonObject& jsonObject);
    SFOPartner(const QVariantMap& dataMap);
    virtual ~SFOPartner();

    QGeoLocation GetLocation() const;

protected:
    virtual QVariantMap _ToVariantMap() const;

    // Server only
    static const QString LatitudeKey;
    static const QString LongitudeKey;

private:
    QGeoLocation _location;
};

typedef QList<SFOPartner *> SFOPartnerList;

#endif // SFOPARTNER_H
