#include "SFOItemModel.h"

#include "SFOContext.h"
#include "SFOPartner.h"

#include <QQmlContext>
#include <QDebug>
#include <QtLocation/QGeoServiceProvider>
#include <QtLocation/QGeoCodingManager>

#include <iostream>

const QString SFOItemModel::ModelIdentifier = "placeModel";
const QString SFOItemModel::PartnerModelIdentifier = "partnerDetail";

SFOItemModel::SFOItemModel(QQmlContext *context, QObject *parent) :
    QAbstractListModel(parent),
    _context(context)
{
    QPlace place;
    place.setLocation(QGeoLocation());
    place.setName("Root");
    _root = place;

    _roleNames[1] = "title";
    _roleNames[2] = "coord";
    _roleNames[3] = "coordinateX";
    _roleNames[4] = "coordinateY";

    SFOContext* ctx = SFOContext::GetInstance();
    // Listen for the partners updated signal so we can updated ourselves
    QObject::connect(ctx, &SFOContext::PartnersUpdated,
                     this, &SFOItemModel::_HandlePartnerUpdate);
    _context->setContextProperty(SFOItemModel::PartnerModelIdentifier, NULL);
}

SFOItemModel::~SFOItemModel() {}

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
    SFOPartnerList partners = SFOContext::GetInstance()->GetPartners();
    if (index.row() < partners.count()) {
        const SFOPartner * partner = partners.at(index.row());
        QGeoLocation location = partner->GetLocation();
        switch(role) {
        case 1:
            return QVariant(partner->GetName_locale());
        case 2:
            // Returns the QGeoCoordinate, which is what
            // QDeclarativeSearchResultModel returns in the place_map example.
            return QVariant::fromValue(location.coordinate());
        case 3:
            return QVariant(location.coordinate().longitude());
        case 4:
            return QVariant(location.coordinate().latitude());
        default:
            std::cerr << "Unknown role " << role << std::endl;
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
    int count = SFOContext::GetInstance()->GetPartners().size();
    return count;
}

QHash<int, QByteArray>
SFOItemModel::roleNames() const
{
    return _roleNames;
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
SFOItemModel::HandleRefresh()
{
    SFOContext::GetInstance()->Refresh();
}

void
SFOItemModel::HandleItemSelected(const QString& title)
{
    qDebug() << "Selected: " << title;
    SFOPartnerList partners = SFOContext::GetInstance()->GetPartners();

    SFOPartner *p;
    foreach(p, partners) {
        if (title == p->GetName_locale()) {
            break;
        }
    }

    _context->setContextProperty(SFOItemModel::PartnerModelIdentifier, NULL);
    _context->setContextProperty(SFOItemModel::PartnerModelIdentifier, p);
}

void
SFOItemModel::_HandlePartnerUpdate()
{
    qDebug() << "updating partner context";
    _context->setContextProperty(SFOItemModel::ModelIdentifier, NULL);
    _context->setContextProperty(SFOItemModel::ModelIdentifier, this);
}
