#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QQmlListProperty>
#include <QSettings>
#include <QTimer>
#include "AudioManager.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"
#include "SettingsManager.h"
#include <qdatetime.h>

#include "EnemyManager.h"

class GameController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int windowWidth READ windowWidth WRITE setWindowWidth NOTIFY windowWidthChanged FINAL)
    Q_PROPERTY(int windowHeight READ windowHeight WRITE setWindowHeight NOTIFY windowHeightChanged FINAL)
    Q_PROPERTY(QQmlListProperty<Bullet> bullets READ bullets NOTIFY bulletsChanged FINAL)
    Q_PROPERTY(int score READ score WRITE setScore NOTIFY scoreChanged FINAL)
    Q_PROPERTY(int highestScore READ highestScore WRITE setHighestScore NOTIFY highestScoreChanged FINAL)
    Q_PROPERTY(int level READ level WRITE setLevel NOTIFY levelChanged FINAL)
    Q_PROPERTY(GameController::GameState gameState READ gameState WRITE setGameState NOTIFY
                   gameStateChanged FINAL)

    Q_PROPERTY(EnemyManager *enemyManager READ enemyManager CONSTANT)

public:
    explicit GameController(Player *player, QObject *parent = nullptr);

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

    Q_INVOKABLE void initialize();

    Q_INVOKABLE void moveReleased();

    Q_INVOKABLE void startGame();
    Q_INVOKABLE void togglePause();
    Q_INVOKABLE void restartGame();
    Q_INVOKABLE void quitGame();

    Q_INVOKABLE void playClickSound();

    Q_INVOKABLE void setPressed(bool pressed);

    double currentX() const;
    void setCurrentX(double newCurrentX);

    double currentY() const;
    void setCurrentY(double newCurrentY);

    void loadSettings(QSettings *settings);

    int windowWidth() const;
    void setWindowWidth(int newWindowWidth);

    int windowHeight() const;
    void setWindowHeight(int newWindowHeight);

    int playerWidth() const;
    void setPlayerWidth(int newPlayerWidth);

    int playerHeight() const;
    void setPlayerHeight(int newPlayerHeight);

    QQmlListProperty<Bullet> bullets();

    int score() const;
    void setScore(int newScore);

    int highestScore() const;
    void setHighestScore(int newHighestScore);

    int level() const;
    void setLevel(int newLevel);

    GameState gameState() const;
    void setGameState(GameState newGameState);

signals:
    void currentXChanged();
    void currentYChanged();

    void windowWidthChanged();

    void windowHeightChanged();

    void playerWidthChanged();

    void playerHeightChanged();

    void bulletsChanged();

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
    double m_playerXOffset {10};
    double m_playerYOffset {0};
    int m_minX {0};
    int m_windowWidth;
    int m_windowHeight;
    float m_gravity {0.5};
    int m_score {0};
    int m_highestScore {0};
    int m_level{1};

    SettingsManager &m_gameControllerSettings = SettingsManager::instance();

    QTimer m_gameTimer;

    QTimer m_thrustTimer;
    QTimer m_enemyCreationTimer;
    QTimer m_collisionTimer;
    QTimer m_playerMoveTimer;
    QTimer m_bulletCreationTimer;

    QQmlListProperty<Bullet> m_bullets;

    Bullet m_bullet;

    EnemyManager *m_enemyManager;

    AudioManager m_audioManager;

    QQmlListProperty<Enemy> m_enemies;

    MoveDirection m_moveDir {MoveDirection::NONE};

    Player *m_player;

    qreal m_velocityX;
    QElapsedTimer m_elapsedTimer;

    void pauseAllTimers();

    EnemyManager *enemyManager() const { return m_enemyManager; }

    bool m_KeyUpPressed{false};

private:
    void checkCollision();
    void checkBulletEnemyCollision();
    void checkEnemyPlayerCollision();
    void gameReset();
    void updateScore();
    GameState m_gameState{GameState::STARTING};
};

#endif // GAMECONTROLLER_H
