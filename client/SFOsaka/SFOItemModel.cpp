#include "SFOItemModel.h"

#include "SFOContext.h"
#include "SFOPartner.h"

#include <QQmlContext>
#include <QDebug>
#include <QtLocation/QGeoServiceProvider>
#include <QtLocation/QGeoCodingManager>

const QString SFOItemModel::PartnerModelIdentifier = "partnerDetail";

const QByteArray SFOItemModel::IndexRole            = "index";
const QByteArray SFOItemModel::TitleRole            = "title";
const QByteArray SFOItemModel::CoordRole            = "coord";
const QByteArray SFOItemModel::StreetRole           = "street";
const QByteArray SFOItemModel::CityRole             = "city";
const QByteArray SFOItemModel::PhoneRole            = "phone";
const QByteArray SFOItemModel::URLRole              = "url";
const QByteArray SFOItemModel::MapMarkerImageRole   = "mapMarkerImage";
const QByteArray SFOItemModel::ImageSourceRole      = "imageSource";
const QByteArray SFOItemModel::DescriptionRole      = "description";
const QByteArray SFOItemModel::ShortDescriptionRole = "shortDescription";

const QString SFOItemModel::_SelectedMarker    = "resources/marker-sel.png";
const QString SFOItemModel::_UnselectedMarker  = "resources/marker.png";

const QHash<int, QByteArray> SFOItemModel::Roles = {
    {0, IndexRole},
    {1, TitleRole},
    {2, CoordRole},
    {3, StreetRole},
    {4, CityRole},
    {5, PhoneRole},
    {6, URLRole},
    {7, MapMarkerImageRole},
    {8, ImageSourceRole},
    {9, DescriptionRole},
    {10, ShortDescriptionRole},
};


SFOItemModel::SFOItemModel(QQmlContext *context, const SFOPartnerList& partners,
                           QObject *parent) :
    QAbstractListModel(parent),
    _context(context),
    _partners(partners),
    _selectedIndex(-1)
{
    QPlace place;
    place.setLocation(QGeoLocation());
    place.setName("Root");
    _root = place;

    _context->setContextProperty(SFOItemModel::PartnerModelIdentifier,
                                 &_emptyPartner);
}

SFOItemModel::~SFOItemModel() {}

SFOPartnerList
SFOItemModel::GetPartners() const
{
    return _partners;
}

void
SFOItemModel::SetPartners(const SFOPartnerList& partners)
{
    if (_partners != partners) {
        _partners = partners;
        _ResetModel();
    }
}

int
SFOItemModel::columnCount(const QModelIndex&) const
{
    return 1;
}

QVariant
SFOItemModel::data(const QModelIndex &index,
                   int role) const
{
    QString localeName = QLocale::system().name();
    if (index.row() < _partners.size()) {
        const SFOPartner * partner = _partners.at(index.row());
        QGeoLocation location = partner->GetLocation();
        switch(role) {
        case 0:                 // IndexRole
            return QVariant(index.row());
        case 1:                 // TitleRole
            return QVariant(partner->GetName_locale());
        case 2:           // CoordRole
            // Returns the QGeoCoordinate, which is what
            // QDeclarativeSearchResultModel returns in the place_map example.
            return QVariant::fromValue(location.coordinate());
        case 3:                 // StreetRole
            return QVariant(partner->GetContactInfoStreet());
        case 4:                 // CityRole
            return QVariant(partner->GetContactInfoCity());
        case 5:                 // PhoneRole
            return QVariant(partner->GetContactInfoPhone());
        case 6:                 // URLRole
            return QVariant(partner->GetURL());
        case 7:                 // MapMarkerImageRole
            return (index.row() == _selectedIndex)
                ? _SelectedMarker
                : _UnselectedMarker;
        case 8:                 // ImageSourceRole
            return QVariant(partner->GetImageURL());
        case 9:                 // DescriptionRole
            return QVariant(partner->GetDescription_locale());
        case 10:
            return QVariant(partner->GetShortDescription_locale());
        default:
            qDebug() << "Unknown role " << role;
            return QVariant();
        }
    }
    return QVariant();
}

QModelIndex
SFOItemModel::parent(const QModelIndex &index) const
{
   if (!index.isValid())
        return QModelIndex();

    return createIndex(0, 0, &_root);
}

int
SFOItemModel::rowCount(const QModelIndex &) const
{
    return _partners.size();
}

QHash<int, QByteArray>
SFOItemModel::roleNames() const
{
    return Roles;
}

QGeoCoordinate
SFOItemModel::GetPosition() const
{
    return _position;
}

void
SFOItemModel::SetPosition(const QGeoCoordinate& pos)
{
    if (pos != _position) {
        _position = pos;
        emit PositionChanged();
    }
}

void
SFOItemModel::HandleItemSelected(const int& selectedIndex)
{
    _selectedIndex = selectedIndex;

    if ((_selectedIndex >= 0) and (_selectedIndex < _partners.size())) {
        SFOPartner *p = _partners[selectedIndex];
        qDebug() << "Setting " << p->GetName() << " as "
                 << SFOItemModel::PartnerModelIdentifier;
        _context->setContextProperty(SFOItemModel::PartnerModelIdentifier, p);
    }
    _ResetModel();
}

void
SFOItemModel::_ResetModel()
{
    qDebug() << "emitting dataChanged";
    emit dataChanged(index(0),index(_partners.size()-1));
}
