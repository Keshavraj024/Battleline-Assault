#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "SettingsManager.h"
#include "GameController.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    SettingsManager gameSettings;

    GameController gameController(gameSettings, &app);

    // PlayerBullet playerBullet(gameSettings, &app);

    engine.rootContext()->setContextProperty("GameController", &gameController);

    // engine.rootContext()->setContextProperty("Bullet", &playerBullet);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Battleline_Assault", "Main");

    return app.exec();
}
