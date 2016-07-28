#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>
#include <QTranslator>
#include <QDebug>

#include "SFOContext.h"
#include "SFOEventFilter.h"
#include "SFOItemModel.h"
#include "SFOScheduleModel.h"
#include "SFOSubmitWordModel.h"
#include "SFOTranslateController.h"
#include "SFOValidator.h"

const QString initialUrl = "http://www.sf-osaka.org/";
//const QString initialUrl = "https://www.futomen.net/";
//const QString initialUrl = "https://www.google.com/";

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QTranslator qtTranslator;
    QString localeName = QLocale::system().name();
    if (!qtTranslator.load("sfosaka_" + localeName,
                         ":/translations/")) {
        qDebug() << "Unable to load translation for " << localeName
                 << " in dir [" << QGuiApplication::applicationDirPath()
                 << "]";
    }
    app.installTranslator(&qtTranslator);

    //    SFOEventFilter filter;
    //app.installEventFilter(&filter);

    QQmlApplicationEngine engine;

    qmlRegisterType<SFOValidator>("SFOsaka", 1, 0, "SFOValidator");
    SFOTranslateController translateController(engine.rootContext());
    SFOScheduleModel festivalScheduleModel(engine.rootContext());

    // Hook up the controller and the event filter
    /*
    QObject::connect(&filter,&SFOEventFilter::TextChanged,
                     &translateController,
                     &SFOTranslateController::OnFilterAccepted);
    */
    SFOItemModel model(engine.rootContext());
    engine.rootContext()->setContextProperty(SFOItemModel::ModelIdentifier,
                                             &model);

    SFOSubmitWordModel submitModel(engine.rootContext());
    engine.rootContext()->setContextProperty("submitModel",
                                             &submitModel);
    QString server = SFOContext::GetInstance()->GetHost();
    engine.rootContext()->setContextProperty("server",
                                             QVariant(server));

    engine.rootContext()->setContextProperty(QStringLiteral("initialUrl"),
                                QUrl::fromUserInput(initialUrl));

    engine.load(QUrl(QStringLiteral("qrc:/sfosaka.qml")));

    return app.exec();
}
