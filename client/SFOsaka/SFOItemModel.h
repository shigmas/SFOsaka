#ifndef SFOITEMMODEL_H
#define SFOITEMMODEL_H

#include "SFOPartner.h"

#include <FJTypes.h>

#include <QObject>
#include <QAbstractListModel>
#include <QPlace>
#include <QPlaceSearchResult>
#include <QGeoCoordinate>

class QQmlContext;

class SFOItemModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QGeoCoordinate position READ GetPosition WRITE SetPosition NOTIFY PositionChanged )

public:
    static const QString PartnerModelIdentifier;

    explicit SFOItemModel(QQmlContext *context, const SFOPartnerList& partners,
                          QObject *parent = Q_NULLPTR);
    virtual ~SFOItemModel();

    SFOPartnerList GetPartners() const;
    void SetPartners(const SFOPartnerList& partners);

    // QAbstractListModel overrides
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index,
                          int role = Qt::DisplayRole) const;

    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    virtual QHash<int, QByteArray> roleNames() const;

    QGeoCoordinate GetPosition() const;
    void SetPosition(const QGeoCoordinate& position);

Q_SIGNALS:
    void PositionChanged();

public slots:
    void HandleItemSelected(const int& selectedIndex);

protected:
    void _ResetModel();
    
protected:
    static const QByteArray IndexRole;
    static const QByteArray TitleRole;
    static const QByteArray CoordRole;
    static const QByteArray StreetRole;
    static const QByteArray CityRole;
    static const QByteArray PhoneRole;
    static const QByteArray URLRole;
    static const QByteArray MapMarkerImageRole;
    static const QByteArray ImageSourceRole;
    static const QByteArray DescriptionRole;
    static const QByteArray ShortDescriptionRole;

    static const QHash<int, QByteArray> Roles;

    static const QString _SelectedMarker;
    static const QString _UnselectedMarker;

private:
    QQmlContext *_context;
    QGeoCoordinate _position;
    SFOPartner _emptyPartner;
    // mutable because creating an index requires a non-const pointer.
    mutable QPlace _root;

    SFOPartnerList _partners;
    int _selectedIndex;
};

#endif // SFOITEMMODEL_H
