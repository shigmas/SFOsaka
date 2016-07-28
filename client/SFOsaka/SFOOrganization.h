#ifndef SFOORGANIZATION_H
#define SFOORGANIZATION_H

#include <QObject>

#include <QDate>
#include <QString>
#include <QGeoLocation>
#include <QJsonObject>

class SFOOrganization : public QObject
{
    Q_OBJECT

    // These fetch the locale specific properties
    Q_PROPERTY(QString name READ GetName_locale NOTIFY NameChanged)
    Q_PROPERTY(QString category READ GetCategory_locale NOTIFY CategoryChanged)
    Q_PROPERTY(QString description READ GetDescription_locale NOTIFY DescriptionChanged)

public:
    explicit SFOOrganization(QObject *parent = 0);

    SFOOrganization(const QJsonObject& jsonObject);
    SFOOrganization(const QVariantMap& dataMap);
    virtual ~SFOOrganization();

    virtual QJsonObject ToJson() const;

    QString GetName_locale() const;
    // We need a string map from the server side values for category in Japanese
    QString GetCategory_locale() const;
    QString GetDescription_locale() const;

    // really a string, but we know it's an id
    QString GetId() const;
    QString GetName() const;
    QString GetNameJp() const;
    QString GetCategory() const;
    QString GetDescription() const;
    QString GetDescriptionJp() const;
    QString GetShortDescription() const;
    QString GetShortDescriptionJp() const;
    QDateTime GetModificationDate() const;
    QString GetURL() const;
    QString GetImageURL() const;

signals:
    void NameChanged();
    void CategoryChanged();
    void DescriptionChanged();
    
protected:
    virtual QVariantMap _ToVariantMap() const;

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
    static const QString URLKey;
    static const QString ImageURLKey;

    // Server only
    static const QString ModificationKey;

private:
    QString _id;
    QString _name;
    QString _nameJp;
    QString _category;
    QString _description;
    QString _descriptionJp;
    QString _shortDescription;
    QString _shortDescriptionJp;
    QDateTime _modificationDate;
    QString _url;
    QString _imageUrl;
};

#endif // SFOORGANIZATION_H
