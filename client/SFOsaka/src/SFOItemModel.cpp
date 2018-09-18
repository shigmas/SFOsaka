#include "SFOItemModel.h"

#include "SFOContext.h"

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
const QByteArray SFOItemModel::BarColorRole         = "barColor";
const QByteArray SFOItemModel::DetailRole           = "detail";

const QString SFOItemModel::_SelectedMarker    = "resources/marker-sel.png";
const QString SFOItemModel::_UnselectedMarker  = "resources/marker.png";

const QHash<int, QByteArray> SFOItemModel::Roles = {
    {Qt::UserRole + 1, IndexRole},
    {Qt::UserRole + 2, TitleRole},
    {Qt::UserRole + 3, CoordRole},
    {Qt::UserRole + 4, StreetRole},
    {Qt::UserRole + 5, CityRole},
    {Qt::UserRole + 6, PhoneRole},
    {Qt::UserRole + 7, URLRole},
    {Qt::UserRole + 8, MapMarkerImageRole},
    {Qt::UserRole + 9, ImageSourceRole},
    {Qt::UserRole + 10, DescriptionRole},
    {Qt::UserRole + 11, ShortDescriptionRole},
    {Qt::UserRole + 12, IsSelectedRole},
    {Qt::UserRole + 13, BarColorRole},
    {Qt::UserRole + 14, DetailRole},
};


SFOItemModel::SFOItemModel(QQmlContext *context,
                           const SFOOrganizationList& organizations,
                           const QString& contextIdentifier,
                           QObject *parent) :
    QAbstractListModel(parent),
    _context(context),
    _organizations(organizations),
    _selectedIndex(-1),
    _contextIdentifier(contextIdentifier)
{
    QPlace place;
    place.setLocation(QGeoLocation());
    place.setName("Root");
    _root = place;

    _SortModel();

    _context->setContextProperty(_contextIdentifier, &_emptyPartner);

}

SFOItemModel::~SFOItemModel() {}

SFOOrganizationList
SFOItemModel::GetOrganizations() const
{
    return _organizations;
}

void
SFOItemModel::SetOrganizations(const SFOOrganizationList& organizations)
{
    if (_organizations != organizations) {
        _organizations = organizations;
        _SortModel();
        // Don't reset the model. The number of organizations may have changed,
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
    if (index.row() < _organizations.size()) {
        const SFOOrganization * organization = _organizations.at(index.row());
        QString selStr = index.row() == _selectedIndex ? "true": "false";
        switch(role) {
        case Qt::UserRole + 1:                 // IndexRole
            return QVariant(index.row());
        case Qt::UserRole + 2:                 // TitleRole
            return QVariant(organization->GetName_locale());
        case Qt::UserRole + 3:           // CoordRole
            return _GetLocation(organization);
        case Qt::UserRole + 4:                 // StreetRole
            return QVariant(organization->GetContactInfoStreet());
        case Qt::UserRole + 5:                 // CityRole
            return QVariant(organization->GetContactInfoCity());
        case Qt::UserRole + 6:                 // PhoneRole
            return QVariant(organization->GetContactInfoPhone());
        case Qt::UserRole + 7:                 // URLRole
            return QVariant(organization->GetURL());
        case Qt::UserRole + 8:                 // MapMarkerImageRole
            return (index.row() == _selectedIndex)
                ? _SelectedMarker
                : _UnselectedMarker;
        case Qt::UserRole + 9:                 // ImageSourceRole
            return QVariant(organization->GetImageURL());
        case Qt::UserRole + 10:                 // DescriptionRole
            return QVariant(organization->GetDescription_locale());
        case Qt::UserRole + 11:                // ShortDescriptionRole
            return QVariant(organization->GetShortDescription_locale());
        case Qt::UserRole + 12:                // IsSelected
            return (index.row() == _selectedIndex)
                ? QVariant(true)
                : QVariant(false);
        case Qt::UserRole + 13:                // ItemBarColorRole
            return organization->GetCategory() == "financial"
                ? QVariant("steelblue")
                : QVariant("lightgray");
        case Qt::UserRole + 14:                 // DetailRole
            //return QVariant(organization->GetDetail_locale());
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
    return _organizations.size();
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
    if ((selectedIndex >= 0) and (selectedIndex < _organizations.size())) {
        SFOOrganization *p = NULL;
        if (selectedIndex != 0) {
            // Move the selected index to the end
            p = _organizations.takeAt(selectedIndex);
            _organizations.prepend(p);
            _selectedIndex = 0;
            
        } else {
            p = _organizations[selectedIndex];
        }
        _context->setContextProperty(_contextIdentifier, p);
    }
}    

// Sorts based on start date, as long as the performer is active.
static bool _isLessThan(const SFOOrganization* first,
                        const SFOOrganization* second)
{
    return first->GetSortOrder() < second->GetSortOrder();
}

void
SFOItemModel::_SortModel()
{
    if ((_organizations.size() == 0) ||
        // first test passed, so there's at least one element in the list
        (!_organizations[0]->IsSorted())) {
        return;
    }

    // Convert to std::list so we can sort
    std::list<SFOOrganization *> orgList = _organizations.toStdList();
    orgList.sort(_isLessThan);
    _organizations = SFOOrganizationList::fromStdList(orgList);
}

void
SFOItemModel::_ResetModel()
{
    emit dataChanged(index(0),index(_organizations.size()-1));
}

QVariant
SFOItemModel::_GetLocation(const SFOOrganization *organization) const
{
    // Returns the QGeoCoordinate, which is what
    // QDeclarativeSearchResultModel returns in the place_map example.
    const SFOPartner *partner = dynamic_cast<const SFOPartner *>(organization);
    if (partner) {
        QGeoLocation location = partner->GetLocation();
        return QVariant::fromValue(location.coordinate());
    } else {
        return QVariant();
    }
}

QVariant
SFOItemModel::_GetDetail(const SFOOrganization *organization) const
{
    // Returns the QGeoCoordinate, which is what
    // QDeclarativeSearchResultModel returns in the place_map example.
    const SFOAppHighlight *appHighlight =
        dynamic_cast<const SFOAppHighlight *>(organization);
    if (appHighlight) {
        return QVariant(appHighlight->GetDetail_locale());
    } else {
        return QVariant();
    }
}
