#ifndef SFOPERFORMER_H
#define SFOPERFORMER_H

#include "SFOOrganization.h"

class SFOPerformer : public SFOOrganization
{
    Q_OBJECT

public:
    explicit SFOPerformer(QObject *parent = 0);

    SFOPerformer(const QString& id, const QString& name, const QString& nameJp,
               const QGeoLocation& location, const QString& category,
               const QString& description, const QString& descriptionJp,
               const QString& shortDescription,
               const QString& shortDescriptionJp);

    SFOPerformer(const QJsonObject& jsonObject);
    SFOPerformer(const QVariantMap& dataMap);
    virtual ~SFOPerformer();

    bool IsActive() const;
    QString GetStatus() const;
    QDateTime GetStartTime() const;
    QDateTime GetEndTime() const;

protected:
    virtual QVariantMap _ToVariantMap() const;

    static const QString StatusKey;
    static const QString StartTimeKey;
    static const QString EndTimeKey;

private:
    QString _status;
    QDateTime _startTime;
    QDateTime _endTime;
};

typedef QList<SFOPerformer *> SFOPerformerList;

#endif // SFOPERFORMER_H
