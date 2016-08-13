#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>
#include <QTranslator>
#include <QDebug>

// Our global controller
#include "SFOController.h"
// For registering the type with the type system
#include "SFOValidator.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Setup for localizations
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

    // Register the validator, to get text from the TextFields without having
    // to wait for Enter
    qmlRegisterType<SFOValidator>("SFOsaka", 1, 0, "SFOValidator");

    SFOController controller(engine.rootContext());

    engine.load(QUrl(QStringLiteral("qrc:/sfosaka.qml")));

    return app.exec();
}
