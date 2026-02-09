#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QQmlListProperty>
#include <QSettings>
#include <QTimer>
#include "AudioManager.h"
#include "Player.h"
#include "SettingsManager.h"
#include <qdatetime.h>

#include "BulletManager.h"
#include "EnemyManager.h"

class GameController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int windowWidth READ windowWidth WRITE setWindowWidth NOTIFY windowWidthChanged FINAL)
    Q_PROPERTY(int windowHeight READ windowHeight WRITE setWindowHeight NOTIFY windowHeightChanged FINAL)

    Q_PROPERTY(int score READ score WRITE setScore NOTIFY scoreChanged FINAL)
    Q_PROPERTY(int highestScore READ highestScore WRITE setHighestScore NOTIFY highestScoreChanged FINAL)
    Q_PROPERTY(int level READ level WRITE setLevel NOTIFY levelChanged FINAL)

    Q_PROPERTY(GameController::GameState gameState READ gameState WRITE setGameState NOTIFY
                   gameStateChanged FINAL)

    Q_PROPERTY(EnemyManager *enemyManager READ enemyManager CONSTANT)
    Q_PROPERTY(BulletManager *bulletManager READ bulletManager CONSTANT)
    Q_PROPERTY(Player *player READ player CONSTANT)

public:
    explicit GameController(QObject *parent = nullptr);

    enum class MoveDirection {
        LEFT,
        RIGHT,
        NONE
    };

    enum class GameState { STARTING, RUNNING, PAUSED, GAMEOVER };
    Q_ENUM(GameState)

    Q_INVOKABLE void moveLeftPressed();
    Q_INVOKABLE void moveRightPressed();
    Q_INVOKABLE void applyBoost();

    Q_INVOKABLE void shootBullet();

    Q_INVOKABLE void moveReleased();

    Q_INVOKABLE void startGame();
    Q_INVOKABLE void togglePause();
    Q_INVOKABLE void restartGame();
    Q_INVOKABLE void quitGame();

    Q_INVOKABLE void playClickSound();

    Q_INVOKABLE void setPressed(bool pressed);

    void initialize();

    int windowWidth() const;
    void setWindowWidth(int newWindowWidth);

    int windowHeight() const;
    void setWindowHeight(int newWindowHeight);

    int score() const;
    void setScore(int newScore);

    int highestScore() const;
    void setHighestScore(int newHighestScore);

    int level() const;
    void setLevel(int newLevel);

    GameState gameState() const;
    void setGameState(GameState newGameState);

    EnemyManager *enemyManager() const;
    BulletManager *bulletManager() const;
    Player *player() const;

signals:
    void windowWidthChanged();

    void windowHeightChanged();

    void scoreChanged();

    void gameOver();

    void highestScoreChanged();

    void levelChanged();

    void gameStateChanged();

private slots:
    void applyGravity();
    void updatePlayerMovement();
    void gameTick();

private:
    SettingsManager &m_gameControllerSettings{SettingsManager::instance()};

    int m_playerminX{0};
    float m_playerYOffset{0.f};
    double m_playerXOffset{10};

    int m_windowWidth;
    int m_windowHeight;

    int m_bulletWidth{0};
    int m_bulletHeight{0};

    int m_enemyWidth{0};
    int m_enemyHeight{0};

    int m_playerWidth{0};
    int m_playerHeight{0};

    float m_gravity{0.5f};
    int m_score{0};
    int m_highestScore {0};
    int m_level{1};

    QTimer m_gameTimer;
    QTimer m_thrustTimer;
    QTimer m_bulletCreationTimer;
    QElapsedTimer m_elapsedTimer;
    QElapsedTimer m_frameTimer;

    Player *m_player;
    EnemyManager *m_enemyManager;
    BulletManager *m_bulletManager;
    AudioManager m_audioManager;

    MoveDirection m_moveDir {MoveDirection::NONE};

    qreal m_playervelocityX;

    float m_timeSinceLastSpawn{0.0f};

    bool m_KeyUpPressed{false};

    float m_spawnEnemyIntervalInSec{6};

    GameState m_gameState{GameState::STARTING};

private:
    void checkCollision();
    void checkBulletEnemyCollision();
    void checkEnemyPlayerCollision();
    void updatePlayerStartingPosition();

    void gameReset();
    void updateScore();
};

#endif // GAMECONTROLLER_H
