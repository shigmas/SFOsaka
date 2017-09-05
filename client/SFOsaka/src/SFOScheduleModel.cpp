#include "SFOScheduleModel.h"

#include "SFOContext.h"
#include "SFOPerformer.h"

#include <QQmlContext>
#include <QDebug>

#include <list>

using std::list;

const QString SFOScheduleModel::ScheduleModelIdentifier = "festival_schedule";

const QByteArray SFOScheduleModel::TitleDisplayRole = "title";
const QByteArray SFOScheduleModel::TimeDisplayRole  = "time";
const QByteArray SFOScheduleModel::DescriptionRole  = "description";
const QByteArray SFOScheduleModel::ImageSourceRole  = "imageSource";

const QHash<int, QByteArray> SFOScheduleModel::Roles = {
    {0, TitleDisplayRole},
    {1, TimeDisplayRole},
    {2, DescriptionRole},
    {3, ImageSourceRole}};

SFOScheduleModel::SFOScheduleModel(QQmlContext *context, QObject *parent) :
    QAbstractListModel(parent),
    _context(context)
{
    SFOContext* ctx = SFOContext::GetInstance();

    _SortPerformers();

    // Listen for the partners updated signal so we can updated ourselves
    QObject::connect(ctx, &SFOContext::PerformersUpdated,
                     this, &SFOScheduleModel::_HandlePerformerUpdate);
    // We set this now, but we will reset it when we change
    _context->setContextProperty(SFOScheduleModel::ScheduleModelIdentifier, this);
}

int
SFOScheduleModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant
SFOScheduleModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < _orderedPerfomers.size()) {
        SFOPerformer *item = _orderedPerfomers.at(index.row());
        switch(role) {
        case 0:
            return item->GetName_locale();
        case 1:
            return QString(item->GetStartTime().toString("h:mma") + " - "
                           + item->GetEndTime().toString("h:mma"));
        case 2:
            return item->GetDescription_locale();
        case 3:
            return item->GetImageURL();
        default:
            qDebug() << "Unknown role " << role;
            return QVariant();
        }
    }
    return QVariant();
}

int
SFOScheduleModel::
rowCount(const QModelIndex &) const
{
    return _orderedPerfomers.size();
}

QHash<int, QByteArray>
SFOScheduleModel::roleNames() const
{
    return Roles;
}

void
SFOScheduleModel::_HandlePerformerUpdate()
{
    // Get the new performers, sort and filter them, and store the order.
    _SortPerformers();
    _context->setContextProperty(SFOScheduleModel::ScheduleModelIdentifier, NULL);
    _context->setContextProperty(SFOScheduleModel::ScheduleModelIdentifier, this);
}

// Sorts based on start date, as long as the performer is active.
static bool _isLessThan(const SFOOrganization* first,
                        const SFOOrganization* second)
{
    const SFOPerformer* pFirst = dynamic_cast<const SFOPerformer *>(first); 
    const SFOPerformer* pSecond = dynamic_cast<const SFOPerformer *>(second);
   
    return pFirst->GetStartTime() < pSecond->GetStartTime();
}

void
SFOScheduleModel::_SortPerformers()
{
    _orderedPerfomers.clear();
    // Boo! No sort on QLists
    std::list<SFOOrganization *> performerList =
        SFOContext::GetInstance()->GetPerformers().toStdList();
    performerList.sort(_isLessThan);

    for(SFOOrganization *organizer : performerList) {
        SFOPerformer* performer = dynamic_cast<SFOPerformer *>(organizer);
        if (performer->IsActive()) {
            _orderedPerfomers.append(performer);
        }
    }    
}
