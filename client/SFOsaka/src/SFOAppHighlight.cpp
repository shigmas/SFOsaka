#include "SFOAppHighlight.h"

#include <QDebug>
#include <QGeoCoordinate>
#include <QVariant>

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
    _detail = dataMap[DetailKey].toString();
    _detailJp = dataMap[DetailJpKey].toString();
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

    dataMap[DetailKey] = _detail;
    dataMap[DetailJpKey] = _detailJp;

    return dataMap;
}
