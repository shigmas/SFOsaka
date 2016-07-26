#include "SFOPartner.h"

#include <QDebug>
#include <QGeoCoordinate>
#include <QVariant>

const QString SFOPartner::LatitudeKey           = "latitude";
const QString SFOPartner::LongitudeKey          = "longitude";

SFOPartner::SFOPartner(QObject *parent) : SFOOrganization(parent)
{

}

SFOPartner::~SFOPartner()
{
}

SFOPartner::SFOPartner(const QJsonObject& jsonObject) :
    SFOPartner(jsonObject.toVariantMap())
{
}

SFOPartner::SFOPartner(const QVariantMap& dataMap) :
    SFOOrganization(dataMap)
{
    double latitude = dataMap[LatitudeKey].toDouble();
    double longitude = dataMap[LongitudeKey].toDouble();
    QGeoCoordinate coord;
    coord.setLatitude(latitude);
    coord.setLongitude(longitude);
    _location.setCoordinate(coord);
}

QGeoLocation
SFOPartner::GetLocation() const
{
    return _location;
}

QVariantMap
SFOPartner::_ToVariantMap() const
{
    QVariantMap dataMap = SFOOrganization::_ToVariantMap();

    double latitude = _location.coordinate().latitude();
    double longitude = _location.coordinate().longitude();
    dataMap[LatitudeKey] = latitude;
    dataMap[LongitudeKey] = longitude;

    return dataMap;
}
