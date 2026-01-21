#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QSettings>
#include <QTimer>
#include <QQmlListProperty>
#include "../entities/PlayerBullet.h"
#include "SettingsManager.h"
#include "../entities/Enemy.h"
#include "../entities/Player.h"

class GameController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int windowWidth READ windowWidth WRITE setWindowWidth NOTIFY windowWidthChanged FINAL)
    Q_PROPERTY(int windowHeight READ windowHeight WRITE setWindowHeight NOTIFY windowHeightChanged FINAL)
    Q_PROPERTY(QQmlListProperty<PlayerBullet> bullets READ bullets NOTIFY bulletsChanged FINAL)
    Q_PROPERTY(QQmlListProperty<Enemy> enemies READ enemies NOTIFY enemiesChanged FINAL)
    Q_PROPERTY(int score READ score WRITE setScore NOTIFY scoreChanged FINAL)
    Q_PROPERTY(int highestScore READ highestScore WRITE setHighestScore NOTIFY highestScoreChanged FINAL)

public:
    explicit GameController(SettingsManager& settings, Player *player, QObject *parent = nullptr);

    enum class MoveDirection {
        LEFT,
        RIGHT,
        NONE
    };

    Q_INVOKABLE void moveLeft();
    Q_INVOKABLE void moveRight();
    Q_INVOKABLE void applyBoost();

    Q_INVOKABLE void shootBullet();
    Q_INVOKABLE void createEnemies();

    Q_INVOKABLE void stopPlayerMoveTimer();

    Q_INVOKABLE void restartGame();

    void initialize();

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

    QQmlListProperty<PlayerBullet> bullets();

    QQmlListProperty<Enemy> enemies();

    int score() const;
    void setScore(int newScore);

    int highestScore() const;
    void setHighestScore(int newHighestScore);

signals:
    void currentXChanged();
    void currentYChanged();

    void windowWidthChanged();

    void windowHeightChanged();

    void playerWidthChanged();

    void playerHeightChanged();

    void bulletsChanged();

    void enemiesChanged();

    void scoreChanged();

    void gameOver();

    void highestScoreChanged();

private slots:
    void applyGravity();
    void destroyBullet(PlayerBullet* bulletToDestroy);
    void destroyEnemy(Enemy* enemyToDestroy);
    void updatePlayerMovement();

private:
    double m_playerXOffset {10};
    double m_playerYOffset {0};
    int m_minX {0};
    int m_windowWidth;
    int m_windowHeight;
    float m_gravity {0.5};
    int m_score {0};
    int m_highestScore {0};

    SettingsManager& m_gameControllerSettings;

    QTimer m_thrustTimer;
    QTimer m_enemyCreationTimer;
    QTimer m_collisionTimer;
    QTimer m_playerMoveTimer;

    QList<PlayerBullet*> m_bulletList {};
    QQmlListProperty<PlayerBullet> m_bullets;

    QList<Enemy*> m_enemyList {};
    QQmlListProperty<Enemy> m_enemies;

    MoveDirection m_moveDir {MoveDirection::NONE};

    Player *m_player;


private:
    void checkCollision();
    void checkBulletEnemyCollision();
    void checkEnemyPlayerCollision();
    void gameReset();
    void updateScore();


};

#endif // GAMECONTROLLER_H
