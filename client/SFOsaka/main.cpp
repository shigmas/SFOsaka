#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTranslator>
#include <QDebug>

#include "SFOTranslateController.h"
#include "SFOItemModel.h"
#include "SFOSubmitWordModel.h"

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
    QQmlApplicationEngine engine;

    SFOTranslateController translateController(engine.rootContext());

    SFOItemModel model(engine.rootContext());
    engine.rootContext()->setContextProperty(SFOItemModel::ModelIdentifier,
                                             &model);

    SFOSubmitWordModel submitModel(engine.rootContext());
    engine.rootContext()->setContextProperty("submitModel",
                                             &submitModel);

    engine.rootContext()->setContextProperty(QStringLiteral("initialUrl"),
                                QUrl::fromUserInput(initialUrl));

    engine.load(QUrl(QStringLiteral("qrc:/sfosaka.qml")));

    return app.exec();
}
