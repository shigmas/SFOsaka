#ifndef SFOAPPHIGHLIGHT_H
#define SFOAPPHIGHLIGHT_H

#include "SFOOrganization.h"

// Everything else include in SFOOrganization.h
#include <QList>

class SFOAppHighlight : public SFOOrganization
{
    Q_OBJECT

public:
    explicit SFOAppHighlight(QObject *parent = 0);

    SFOAppHighlight(const QJsonObject& jsonObject);
    SFOAppHighlight(const QVariantMap& dataMap);
    virtual ~SFOAppHighlight();

    QString GetDetail_locale() const;
    QString GetDetail() const;
    QString GetDetailJp() const;

protected:
    virtual QVariantMap _ToVariantMap() const;

    // Server only
    static const QString DetailKey;
    static const QString DetailJpKey;

private:
    QString _detail;
    QString _detailJp;
};

// We're not really trying to be polymorphic here, just making use of common
// code. So, SFOOrganization is really a hidden class, and we use the subclasses
// directly.
typedef QList<SFOAppHighlight *> SFOAppHighlightList;

#endif // SFOAPPHIGHLIGHT_H
