#include "SFOAppHighlight.h"

#include <QDebug>
#include <QGeoCoordinate>
#include <QVariant>

const QString SFOAppHighlight::OrderKey             = "order";
const QString SFOAppHighlight::DetailKey            = "detail";
const QString SFOAppHighlight::DetailJpKey          = "detail_jp";

SFOAppHighlight::SFOAppHighlight(QObject *parent) : SFOOrganization(parent)
{

}

SFOAppHighlight::~SFOAppHighlight()
{
}

SFOAppHighlight::SFOAppHighlight(const QJsonObject& jsonObject) :
    SFOAppHighlight(jsonObject.toVariantMap())
{
}

SFOAppHighlight::SFOAppHighlight(const QVariantMap& dataMap) :
    SFOOrganization(dataMap)
{
    _order = dataMap[OrderKey].toUInt();
    _detail = dataMap[DetailKey].toString();
    _detailJp = dataMap[DetailJpKey].toString();
}

bool
SFOAppHighlight::IsSorted() const
{
    return true;
}

quint16
SFOAppHighlight::GetSortOrder() const
{
    return GetOrder();
}

quint16
SFOAppHighlight::GetOrder() const
{
    return _order;
}

QString
SFOAppHighlight::GetDetail_locale() const
{
    QString localeName = QLocale::system().name();
    if (localeName == "ja_JP") {
        return GetDetailJp();
    } else {
        return GetDetail();
    }
}

QString
SFOAppHighlight::GetDetail() const
{
    return _detail;
}

QString
SFOAppHighlight::GetDetailJp() const
{
    return _detailJp;
}

QVariantMap
SFOAppHighlight::_ToVariantMap() const
{
    QVariantMap dataMap = SFOOrganization::_ToVariantMap();

    dataMap[OrderKey] = _order;
    dataMap[DetailKey] = _detail;
    dataMap[DetailJpKey] = _detailJp;

    return dataMap;
}
