#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "GameController.h"
#include "Player.h"
#include "SettingsManager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    Player *player = new Player(&app);

    GameController gameController(player, &app);

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
