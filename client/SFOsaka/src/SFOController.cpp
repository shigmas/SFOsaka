#include "SFOController.h"

#include "SFOContext.h"
#include "SFOEventFilter.h"
#include "SFOItemModel.h"
#include "SFOScheduleModel.h"
#include "SFOSubmitWordModel.h"
#include "SFOTranslateController.h"

#include <QQmlContext>
#include <QDebug>


SFOController::SFOController(QQmlContext *context, QObject *parent) :
    QObject(parent),
    _context(context)
{
    _context->setContextProperty("globalController", this);

    _translateController = new SFOTranslateController(_context);
    _festivalScheduleModel = new SFOScheduleModel(_context);
    _submitWordModel = new SFOSubmitWordModel(_context);

    _context->setContextProperty("submitModel", _submitWordModel);

    SFOOrganizationList restaurants =
        SFOContext::GetInstance()->GetPartnersByCategory(SFOFoodCategory);
    _foodPartnersModel = new SFOItemModel(_context, restaurants);
    _context->setContextProperty("placeModel", _foodPartnersModel);

    SFOOrganizationList otherPartners =
        SFOContext::GetInstance()->GetNonFoodPartners();
    _otherPartnersModel = new SFOItemModel(_context, otherPartners);
    _context->setContextProperty("partnersModel", _otherPartnersModel);

    SFOOrganizationList appHighlights =
        SFOContext::GetInstance()->GetAppHighlights();
    _appHighlightsModel = new SFOItemModel(_context, appHighlights);
    _context->setContextProperty("appHighlightsModel", _appHighlightsModel);

    QString server = SFOContext::GetInstance()->GetHost();
    _context->setContextProperty("server", QVariant(server));

    // Hook up to the context signals to update the partner models
    SFOContext *sfoContext = SFOContext::GetInstance();
    QObject::connect(sfoContext,&SFOContext::PartnersUpdated,
                     this, &SFOController::_OnPartnersUpdated);

}

SFOController::~SFOController()
{
    delete _translateController;
    delete _festivalScheduleModel;
    delete _submitWordModel;
    delete _foodPartnersModel;
    delete _otherPartnersModel;
    delete _appHighlightsModel;
}


void
SFOController::HandleRefresh()
{
    qDebug() << "HandleRefresh";
    SFOContext::GetInstance()->Refresh(true);
}

void
SFOController::_OnPartnersUpdated()
{
    qDebug() << "Handling partners updated.";
    SFOOrganizationList restaurants =
        SFOContext::GetInstance()->GetPartnersByCategory(SFOFoodCategory);
    _foodPartnersModel->SetOrganizations(restaurants);
    // In this case, we have to force a model reload, because the size has
    // changed
    _context->setContextProperty("placeModel", NULL);
    _context->setContextProperty("placeModel", _foodPartnersModel);
    SFOOrganizationList otherPartners =
        SFOContext::GetInstance()->GetNonFoodPartners();
    _otherPartnersModel->SetOrganizations(otherPartners);
    _context->setContextProperty("partnersModel", NULL);
    _context->setContextProperty("partnersModel", _otherPartnersModel);
    _context->setContextProperty("appHighlightsModel", NULL);
    _context->setContextProperty("appHighlightsModel", _appHighlightsModel);
}
