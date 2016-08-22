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
const QByteArray SFOItemModel::IsSelectedRole       = "isSelected";

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
    {11, IsSelectedRole},
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
        // Don't reset the model. The number of partners may have changed,
        // and that could cause a crash. Setting and resetting the model (which
        // we should do, but is done in the controller right now) is sufficient.
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
        QString selStr = index.row() == _selectedIndex ? "true": "false";
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
        case 10:                // ShortDescriptionRole
            return QVariant(partner->GetShortDescription_locale());
        case 11:                // IsSelected
            return (index.row() == _selectedIndex)
                ? QVariant(true)
                : QVariant(false);
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
    _SetItemAsSelected(_selectedIndex);
    _ResetModel();
}

void
SFOItemModel::ToggleItemSelected(const int& selectedIndex)
{
    // If it's already selected, set no selection. Otherwise, select it.
    _selectedIndex = _selectedIndex == selectedIndex
        ? -1
        : selectedIndex;
    _SetItemAsSelected(_selectedIndex);
    _ResetModel();
}

void
SFOItemModel::_SetItemAsSelected(const int& selectedIndex)
{
    if ((selectedIndex >= 0) and (selectedIndex < _partners.size())) {
        SFOPartner *p = NULL;
        if (selectedIndex != _partners.size() - 1) {
            // Move the selected index to the end
            p = _partners.takeAt(selectedIndex);
            _partners.prepend(p);
            _selectedIndex = 0;
            
        } else {
            p = _partners[selectedIndex];
        }
        _context->setContextProperty(SFOItemModel::PartnerModelIdentifier, p);
    }
}    

void
SFOItemModel::_ResetModel()
{
    emit dataChanged(index(0),index(_partners.size()-1));
}
