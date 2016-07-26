#include "SFOPerformer.h"

#include <QDebug>
#include <QGeoCoordinate>
#include <QVariant>

const QString SFOPerformer::StatusKey     = "status";
const QString SFOPerformer::StartTimeKey  = "startTime";
const QString SFOPerformer::EndTimeKey    = "endTime";

SFOPerformer::SFOPerformer(QObject *parent) : SFOOrganization(parent)
{

}

SFOPerformer::~SFOPerformer()
{
}

SFOPerformer::SFOPerformer(const QJsonObject& jsonObject) :
    SFOPerformer(jsonObject.toVariantMap())
{
}

SFOPerformer::SFOPerformer(const QVariantMap& dataMap) :
    SFOOrganization(dataMap)
{
    _status = dataMap[StatusKey].toString();
    QString date = dataMap[StartTimeKey].toString();
    _startTime = QDateTime::fromString(date,Qt::ISODate);
    date = dataMap[EndTimeKey].toString();
    _endTime = QDateTime::fromString(date,Qt::ISODate);
}

bool
SFOPerformer::IsActive() const
{
    return _status == "active";
}

QString
SFOPerformer::GetStatus() const
{
    return _status;
}

QDateTime
SFOPerformer::GetStartTime() const
{
    return _startTime;
}

QDateTime
SFOPerformer::GetEndTime() const
{
    return _endTime;
}

QVariantMap
SFOPerformer::_ToVariantMap() const
{
    QVariantMap dataMap = SFOOrganization::_ToVariantMap();

    dataMap[StatusKey] = _status;
    dataMap[StartTimeKey] = _startTime.toString(Qt::ISODate);
    dataMap[EndTimeKey] = _endTime.toString(Qt::ISODate);

    return dataMap;
}
