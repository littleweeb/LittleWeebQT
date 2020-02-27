#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QObject>

//include view controllers
#include "Headers/QmlControllers/ircconnectformcontroller.h"
#include "Headers/QmlControllers/ircchatformcontroller.h"
#include "Headers/QmlControllers/dccdownloadrequestcontroller.h"

//include services
#include "Headers/IrcClient/ircclient.h"
#include "Headers/DccClient/dccclient.h"

int main(int argc, char *argv[])
{

    //Instantiate services
    IrcClient ircClient;
    DccClient dccClient;

    // Link services
    ircClient.registerDccClient(&dccClient);

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Register Types
    qmlRegisterType<DccDownloadObj>("com.dcc.dccdownloadobj", 1, 0, "DccDownloadObj");
    qmlRegisterType<DccDownloadProgress>("com.dcc.dccdownloadprogress", 1, 0, "DccDownloadProgress");
    qmlRegisterType<DccDownloadStatus>("com.dcc.dccdownloadstatus", 1, 0, "DccDownloadStatus");

    // Instantiate controllers
    IrcConnectFormController ircConnectFormController;
    IrcChatFormController ircChatFormController;
    DccDownloadRequestController dccDownloadRequestController;

    // Link controllers to services
    ircConnectFormController.registerIrcClient(&ircClient);
    ircChatFormController.registerIrcClient(&ircClient);
    dccDownloadRequestController.registerDccClient(&dccClient);

    auto root_context = engine.rootContext();
    root_context->setContextProperty("_ircConnectFormController", &ircConnectFormController);
    root_context->setContextProperty("_ircChatFormController", &ircChatFormController);
    root_context->setContextProperty("_dccDownloadRequestController", &dccDownloadRequestController);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);


    return app.exec();
}
