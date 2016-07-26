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

// We're not really trying to be polymorphic here, just making use of common
// code. So, SFOOrganization is really a hidden class, and we use the subclasses
// directly.
typedef QList<SFOPartner *> SFOPartnerList;

#endif // SFOPARTNER_H
