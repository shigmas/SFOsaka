#ifndef SFOCONTROLLER_H
#define SFOCONTROLLER_H

#include <QObject>

///
// Global controller. This handles parcelling out data to the various objects
// and models, as well as updating the models when the data changes.
// Most of the stuff that was growing in main.cpp was moved here. Since this
// is QObject-derived, we can handle and emit signals.
//
// We're not very consistent with what objects take care of things by themselves
// and which ones rely on this controller to update them. In general, we are
// hands off (since that's the way we were doing things before), but for the
// SFOItemModel, it doesn't know what kinds of items it holds. While they are
// SFOPartners currently, it could be expanded SFOPartner's base class,
// SFOOrganization. But, the SFOItemModel doesn't know which category it
// holds. It just passes back the data that it holds, and handles selection.
//

class SFOTranslateController;
class SFOScheduleModel;
class SFOSubmitWordModel;
class SFOItemModel;

class QQmlContext;

class SFOController : public QObject
{
    Q_OBJECT
public:
    explicit SFOController(QQmlContext *context, QObject *parent = 0);
    ~SFOController();

signals:

public slots:
    void HandleRefresh();

protected slots:
    void _OnPartnersUpdated();
    void _OnAppHighlightsUpdated();

private:
    QQmlContext *_context;

    SFOTranslateController *_translateController;
    SFOScheduleModel *_festivalScheduleModel;
    SFOSubmitWordModel *_submitWordModel;
    SFOItemModel *_foodPartnersModel;
    SFOItemModel *_otherPartnersModel;
    SFOItemModel *_appHighlightsModel;
};

#endif // SFOCONTROLLER_H
