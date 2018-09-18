#include "SFOTransportation.h"

#include <QDebug>
#include <QGeoCoordinate>
#include <QVariant>

const QString SFOTransportation::OrderKey             = "order";
const QString SFOTransportation::DetailKey            = "detail";
const QString SFOTransportation::DetailJpKey          = "detail_jp";

SFOTransportation::SFOTransportation(QObject *parent) : SFOOrganization(parent)
{

}

SFOTransportation::~SFOTransportation()
{
}

SFOTransportation::SFOTransportation(const QJsonObject& jsonObject) :
    SFOTransportation(jsonObject.toVariantMap())
{
}

SFOTransportation::SFOTransportation(const QVariantMap& dataMap) :
    SFOOrganization(dataMap)
{
    _order = dataMap[OrderKey].toUInt();
    _detail = dataMap[DetailKey].toString();
    _detailJp = dataMap[DetailJpKey].toString();
}

bool
SFOTransportation::IsSorted() const
{
    return true;
}

quint16
SFOTransportation::GetSortOrder() const
{
    return GetOrder();
}

quint16
SFOTransportation::GetOrder() const
{
    return _order;
}

QString
SFOTransportation::GetDetail_locale() const
{
    QString localeName = QLocale::system().name();
    if (localeName == "ja_JP") {
        return GetDetailJp();
    } else {
        return GetDetail();
    }
}

QString
SFOTransportation::GetDetail() const
{
    return _detail;
}

QString
SFOTransportation::GetDetailJp() const
{
    return _detailJp;
}

QVariantMap
SFOTransportation::_ToVariantMap() const
{
    QVariantMap dataMap = SFOOrganization::_ToVariantMap();

    dataMap[OrderKey] = _order;
    dataMap[DetailKey] = _detail;
    dataMap[DetailJpKey] = _detailJp;

    return dataMap;
}
