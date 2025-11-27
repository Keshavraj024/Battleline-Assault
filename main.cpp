#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "SettingsManager.h"
#include "GameController.h"
#include "Player.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    SettingsManager gameSettings;

    Player *player = new Player(gameSettings, &app);

    GameController gameController(gameSettings, player, &app);

    engine.rootContext()->setContextProperty("GameController", &gameController);

    engine.rootContext()->setContextProperty("PlayerController", player);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Battleline_Assault", "Main");

    return app.exec();
}
