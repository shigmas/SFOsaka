#ifndef SFOPARTNER_H
#define SFOPARTNER_H

#include <QObject>

#include <QDate>
#include <QString>
#include <QGeoLocation>
#include <QJsonObject>
#include <QList>

class SFOPartner : public QObject
{
    Q_OBJECT

    // These fetch the locale specific properties
    Q_PROPERTY(QString name READ GetName_locale)
    Q_PROPERTY(QString category READ GetCategory_locale)
    Q_PROPERTY(QString description READ GetDescription_locale)

public:
    explicit SFOPartner(QObject *parent = 0);

    SFOPartner(const QString& id, const QString& name, const QString& nameJp,
               const QGeoLocation& location, const QString& category,
               const QString& description, const QString& descriptionJp,
               const QString& shortDescription,
               const QString& shortDescriptionJp);

    SFOPartner(const QJsonObject& jsonObject);
    SFOPartner(const QVariantMap& dataMap);
    virtual ~SFOPartner();

    QJsonObject ToJson() const;

    QString GetName_locale() const;
    // We need a string map from the server side values for category in Japanese
    QString GetCategory_locale() const;
    QString GetDescription_locale() const;

    // really a string, but we know it's an id
    QString GetId() const;
    QString GetName() const;
    QString GetNameJp() const;
    QGeoLocation GetLocation() const;
    QString GetCategory() const;
    QString GetDescription() const;
    QString GetDescriptionJp() const;
    QString GetShortDescription() const;
    QString GetShortDescriptionJp() const;
    QDateTime GetModificationDate() const;

protected:
    // Mixed strings with server protocol and our JSON keys on disk. Mostly
    // the same unless indicated
    static const QString IdKey;
    static const QString NameKey;
    static const QString NameJpKey;
    static const QString CategoryKey;
    static const QString DescriptionKey;
    static const QString DescriptionJpKey;
    static const QString ShortDescriptionKey;
    static const QString ShortDescriptionJpKey;

    // Server only
    static const QString LatitudeKey;
    static const QString LongitudeKey;
    static const QString ModificationKey;

private:
    QString _id;
    QString _name;
    QString _nameJp;
    QGeoLocation _location;
    QString _category;
    QString _description;
    QString _descriptionJp;
    QString _shortDescription;
    QString _shortDescriptionJp;
    QDateTime _modificationDate;
};

typedef QList<SFOPartner *> SFOPartnerList;

#endif // SFOPARTNER_H
