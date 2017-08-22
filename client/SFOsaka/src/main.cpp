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

// To work around a bug in Qt
#include "SFOIgnoreNetworkFactory.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }
}


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qInstallMessageHandler(myMessageOutput);

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
    engine.setNetworkAccessManagerFactory(new SFOIgnoreNetworkFactory);
    // Register the validator, to get text from the TextFields without having
    // to wait for Enter
    qmlRegisterType<SFOValidator>("SFOsaka", 1, 0, "SFOValidator");

    SFOController controller(engine.rootContext());

    engine.load(QUrl(QStringLiteral("qrc:/qml/sfosaka.qml")));

    return app.exec();
}
