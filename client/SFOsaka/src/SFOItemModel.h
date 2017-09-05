#ifndef SFOITEMMODEL_H
#define SFOITEMMODEL_H

#include "SFOOrganization.h"
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
    // Legacy constant, when this is used as the detail for map views
    static const QString PartnerModelIdentifier;

    explicit SFOItemModel(QQmlContext *context,
                          const SFOOrganizationList& organizations,
                          const QString& contextIdentifier = PartnerModelIdentifier,
                          QObject *parent = Q_NULLPTR);
    virtual ~SFOItemModel();

    SFOOrganizationList GetOrganizations() const;
    void SetOrganizations(const SFOOrganizationList& organizations);

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
    void ToggleItemSelected(const int& selectedIndex);

protected:
    void _SetItemAsSelected(const int& selectedIndex);
    void _ResetModel();

    // These apply to certain subclasses of SFOOrganization. If they're not
    // appicable, they return an empty QVariant. It's up to the callers to
    // know if the data they want is supplied by that subclass.
    QVariant _GetLocation(const SFOOrganization *organization) const;
    QVariant _GetDetail(const SFOOrganization *organization) const;
    
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
    static const QByteArray IsSelectedRole;

    // XXX - Last minute hack to get the release out today
    static const QByteArray BarColorRole;

    // AppHighlight extension
    static const QByteArray DetailRole;

    // This model is used for all Organization subclasses, so we have roles
    // for all properties.
    static const QHash<int, QByteArray> Roles;

    static const QString _SelectedMarker;
    static const QString _UnselectedMarker;

private:
    QQmlContext *_context;
    QGeoCoordinate _position;
    SFOPartner _emptyPartner;
    // mutable because creating an index requires a non-const pointer.
    mutable QPlace _root;

    SFOOrganizationList _organizations;
    int _selectedIndex;

    QString _contextIdentifier;
};

#endif // SFOITEMMODEL_H
