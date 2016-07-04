#include "SFOPartner.h"

#include <QDebug>
#include <QGeoCoordinate>
#include <QVariant>

const QString SFOPartner::IdKey                 = "id";
const QString SFOPartner::NameKey               = "name";
const QString SFOPartner::NameJpKey             = "name_jp";
const QString SFOPartner::CategoryKey           = "category";
const QString SFOPartner::DescriptionKey        = "description";
const QString SFOPartner::DescriptionJpKey      = "description_jp";
const QString SFOPartner::ShortDescriptionKey   = "shortDescription";
const QString SFOPartner::ShortDescriptionJpKey = "shortDescription_jp";

const QString SFOPartner::LatitudeKey           = "latitude";
const QString SFOPartner::LongitudeKey          = "longitude";
const QString SFOPartner::ModificationKey       = "modificationDate";

SFOPartner::SFOPartner(QObject *parent) : QObject(parent)
{

}


SFOPartner::SFOPartner(const QString& id, const QString& name,
                       const QString& nameJp, const QGeoLocation& location,
                       const QString& category,
                       const QString& description, const QString& descriptionJp,
                       const QString& shortDescription,
                       const QString& shortDescriptionJp) :
    _id(id), _name(name), _nameJp(nameJp), _location(location),
    _category(category), _description(description), _descriptionJp(descriptionJp),
    _shortDescription(shortDescription), _shortDescriptionJp(shortDescriptionJp)
{
}

SFOPartner::SFOPartner(const QJsonObject& jsonObject) :
    SFOPartner(jsonObject.toVariantMap())
{
}

SFOPartner::~SFOPartner()
{
}

SFOPartner::SFOPartner(const QVariantMap& dataMap)
{
    _id = dataMap[IdKey].toString();
    _name = dataMap[NameKey].toString();
    _nameJp = dataMap[NameJpKey].toString();
    double latitude = dataMap[LatitudeKey].toDouble();
    double longitude = dataMap[LongitudeKey].toDouble();
    QGeoCoordinate coord;
    coord.setLatitude(latitude);
    coord.setLongitude(longitude);
    _location.setCoordinate(coord);
    _category = dataMap[CategoryKey].toString();
    _description = dataMap[DescriptionKey].toString();
    _descriptionJp = dataMap[DescriptionJpKey].toString();
    _shortDescription = dataMap[ShortDescriptionKey].toString();
    _shortDescriptionJp = dataMap[ShortDescriptionJpKey].toString();
    QString modDate = dataMap[ModificationKey].toString();
    _modificationDate = QDateTime::fromString(modDate,Qt::ISODate);
}

QJsonObject
SFOPartner::ToJson() const
{
    QVariantMap dataMap;

    dataMap[IdKey] = _id;
    dataMap[NameKey] = _name;
    dataMap[NameJpKey] = _nameJp;
    double latitude = _location.coordinate().latitude();
    double longitude = _location.coordinate().longitude();
    dataMap[LatitudeKey] = latitude;
    dataMap[LongitudeKey] = longitude;
    dataMap[CategoryKey] = _category;
    dataMap[DescriptionKey] = _description;
    dataMap[DescriptionJpKey] = _descriptionJp;
    dataMap[ShortDescriptionKey] = _shortDescription;
    dataMap[ShortDescriptionJpKey] = _shortDescriptionJp;
    dataMap[ModificationKey] = _modificationDate.toString(Qt::ISODate);

    return QJsonObject::fromVariantMap(dataMap);
}

QString
SFOPartner::GetName_locale() const
{
    QString localeName = QLocale::system().name();
    if (localeName == "ja_JP") {
        return GetNameJp();
    } else {
        return GetName();
    }
}

QString
SFOPartner::GetCategory_locale() const
{
    return GetCategory();
}

QString
SFOPartner::GetDescription_locale() const
{
    QString localeName = QLocale::system().name();
    if (localeName == "ja_JP") {
        return GetDescriptionJp();
    } else {
        return GetDescription();
    }
}

QString
SFOPartner::GetId() const
{
    return _id;
}

QString
SFOPartner::GetName() const
{
    return _name;
}

QString
SFOPartner::GetNameJp() const
{
    return _nameJp;
}

QGeoLocation
SFOPartner::GetLocation() const
{
    return _location;
}

QString
SFOPartner::GetCategory() const
{
    return _category;
}

QString
SFOPartner::GetDescription() const
{
    return _description;
}

QString
SFOPartner::GetDescriptionJp() const
{
    return _descriptionJp;
}

QString
SFOPartner::GetShortDescription() const
{
    return _shortDescription;
}

QString
SFOPartner::GetShortDescriptionJp() const
{
    return _shortDescriptionJp;
}

QDateTime
SFOPartner::GetModificationDate() const
{
    return _modificationDate;
}
