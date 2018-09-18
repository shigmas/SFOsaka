#ifndef SFOTRANSPORTATION_H
#define SFOTRANSPORTATION_H

#include "SFOOrganization.h"

// Everything else include in SFOOrganization.h
#include <QList>

class SFOTransportation : public SFOOrganization
{
    Q_OBJECT

public:
    explicit SFOTransportation(QObject *parent = 0);

    SFOTransportation(const QJsonObject& jsonObject);
    SFOTransportation(const QVariantMap& dataMap);
    virtual ~SFOTransportation();

    virtual bool IsSorted() const;
    virtual quint16 GetSortOrder() const;

    quint16 GetOrder() const;
    QString GetDetail_locale() const;
    QString GetDetail() const;
    QString GetDetailJp() const;

protected:
    virtual QVariantMap _ToVariantMap() const;

    // Server only
    static const QString OrderKey;
    static const QString DetailKey;
    static const QString DetailJpKey;

private:
    quint16 _order;
    QString _detail;
    QString _detailJp;
};

// We're not really trying to be polymorphic here, just making use of common
// code. So, SFOOrganization is really a hidden class, and we use the subclasses
// directly.
typedef QList<SFOTransportation *> SFOTransportationList;

#endif // SFOTRANSPORTATION_H
