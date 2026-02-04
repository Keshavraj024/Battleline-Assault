#include "GameController.h"
#include <QCoreApplication>
#include <QRandomGenerator>
#include <QRectF>

constexpr int SCORE_PER_KILL = 10;
constexpr int LEVEL_UP_ENTRY = 100;
constexpr int BASE_INTERVAL = 2000;
constexpr int DECREASE_PER_LEVEL = 100;
constexpr int MIN_INTERVAL = 200;
constexpr qreal PLAYER_MAX_SPEED = 500.0;
constexpr qreal PLAYER_ACCL = 3000.0;
constexpr qreal PLAYER_FRICTION = 2500.0;

GameController::GameController(Player *player, QObject *parent)
    : QObject{parent}
    , m_player(player)
{
    m_windowWidth = m_gameControllerSettings.getValue("window/width").toInt();
    m_windowHeight = m_gameControllerSettings.getValue("window/height").toInt();

    m_highestScore = m_gameControllerSettings.getValue("game/highestScore").toInt();

    connect(&m_thrustTimer, &QTimer::timeout, this, &GameController::applyGravity);
    m_thrustTimer.setInterval(16);

    connect(&m_enemyCreationTimer, &QTimer::timeout, this, [this]() {
        m_enemy.addEnemy();
        emit enemiesChanged();
    });
    m_enemyCreationTimer.setInterval(BASE_INTERVAL);

    connect(&m_collisionTimer, &QTimer::timeout, this, &GameController::checkCollision);
    m_collisionTimer.setInterval(16);

    connect(&m_playerMoveTimer, &QTimer::timeout, this, &GameController::updatePlayerMovement);
    m_playerMoveTimer.setInterval(16);
}

void GameController::initialize()
{
    // for (auto &timer : m_timerVector)
    //     timer.start();
    m_enemyCreationTimer.start();
    m_playerMoveTimer.start();
    m_collisionTimer.start();

    m_elapsedTimer.start();

    setScore(m_score);
    setLevel(m_level);

    m_player->initialize();

    m_bulletCreationTimer.setSingleShot(true);

    m_gameControllerSettings.setValue("player/startX", m_player->playerCurrentX());
    m_gameControllerSettings.setValue("player/startY", m_player->playerCurrentY());
}

void GameController::moveReleased()
{
    m_moveDir = MoveDirection::NONE;
}

void GameController::startGame()
{
    setGameState(GameState::RUNNING);
    initialize();
}

void GameController::pauseGame()
{
    if (m_gameState == GameState::RUNNING) {
        pauseAllTimers();
        setGameState(GameState::PAUSED);
    }
}

void GameController::resumeGame()
{
    if (m_gameState == GameState::PAUSED) {
        m_enemyCreationTimer.start();
        m_playerMoveTimer.start();
        m_collisionTimer.start();

        m_enemy.resumeEnemyFallTimer();
        m_bullet.resumeBulletFallTimer();

        setGameState(GameState::RUNNING);
    }
}

void GameController::restartGame()
{
    gameReset();
    startGame();
}

void GameController::quitGame()
{
    QCoreApplication::quit();
}

void GameController::playClickSound()
{
    m_audioManager.playClick();
}

void GameController::moveLeftPressed()
{
    m_moveDir = MoveDirection::LEFT;
}

void GameController::moveRightPressed()
{
    m_moveDir = MoveDirection::RIGHT;
}

int GameController::windowWidth() const
{
    return m_windowWidth;
}

void GameController::setWindowWidth(int newWindowWidth)
{
    if (m_windowWidth == newWindowWidth)
        return;
    m_windowWidth = newWindowWidth;
    emit windowWidthChanged();
}

int GameController::windowHeight() const
{
    return m_windowHeight;
}

void GameController::setWindowHeight(int newWindowHeight)
{
    if (m_windowHeight == newWindowHeight)
        return;
    m_windowHeight = newWindowHeight;
    emit windowHeightChanged();
}

void GameController::applyBoost()
{
    m_playerYOffset = -10;
    if (!m_thrustTimer.isActive())
        m_thrustTimer.start(); // 60 FPS
}

void GameController::shootBullet()
{
    if (!m_bulletCreationTimer.isActive()) {
        m_audioManager.playShoot();

        m_gameControllerSettings.setValue("player/startX", m_player->playerCurrentX());
        m_gameControllerSettings.setValue("player/startY", m_player->playerCurrentY());

        m_bullet.addBullet();
        emit bulletsChanged();
        m_bulletCreationTimer.start(50);
    }
}

void GameController::applyGravity()
{
    double newYPos = m_player->playerCurrentY() + m_playerYOffset;

    if (newYPos > (m_windowHeight - m_player->playerHeight())) {
        newYPos = m_windowHeight - m_player->playerHeight();
        if (m_thrustTimer.isActive())
            m_thrustTimer.stop();
    }

    if (newYPos < m_windowHeight / 2)
        newYPos = m_windowHeight / 2;

    m_player->setPlayerCurrentY(newYPos);
    m_playerYOffset += m_gravity;
}

void GameController::updatePlayerMovement()
{
    qreal dt = m_elapsedTimer.restart() / 1000.0; //(sec)

    qreal input = 0.0;
    if (m_moveDir == MoveDirection::LEFT)
        input = -1.0;

    if (m_moveDir == MoveDirection::RIGHT)
        input = 1.0;

    if (input != 0.0)
        m_velocityX += PLAYER_ACCL * input * dt;
    else {
        if (m_velocityX < 0.0)
            m_velocityX = std::min(0.0, m_velocityX + PLAYER_FRICTION * dt);
        else if (m_velocityX > 0.0)
            m_velocityX = std::max(0.0, m_velocityX - PLAYER_FRICTION * dt);
    }

    m_velocityX = std::clamp(m_velocityX, -PLAYER_MAX_SPEED, PLAYER_MAX_SPEED);

    int newPos = m_player->playerCurrentX() + m_velocityX * dt;

    newPos = std::clamp(newPos, m_minX, m_windowWidth - m_player->playerWidth());
    m_player->setPlayerCurrentX(newPos);
}

void GameController::pauseAllTimers()
{
    m_enemyCreationTimer.stop();
    m_playerMoveTimer.stop();
    m_collisionTimer.stop();

    m_enemy.stopEnemyFallTimer();
    m_bullet.stopBulletFallTimer();
}

void GameController::checkCollision()
{
    checkBulletEnemyCollision();
    checkEnemyPlayerCollision();
}

bool isCollided(PlayerBullet *bullet, Enemy *enemy)
{
    QRectF bulletRect{bullet->bulletX(),
                      bullet->bulletY(),
                      static_cast<qreal>(bullet->bulletWidth()),
                      static_cast<qreal>(bullet->bulletHeight())};
    QRectF enemyRect{enemy->enemyX(),
                     enemy->enemyY(),
                     static_cast<qreal>(enemy->enemyWidth()),
                     static_cast<qreal>(enemy->enemyHeight())};

    if (bulletRect.intersects(enemyRect)) {
        return true;
    }
    return false;
}

void GameController::updateScore()
{
    setScore(m_score + SCORE_PER_KILL);

    if (m_score % LEVEL_UP_ENTRY == 0) {
        setLevel(m_level + 1);
        auto setInterval = std::max(200, BASE_INTERVAL - (DECREASE_PER_LEVEL * m_level));
        m_enemyCreationTimer.setInterval(setInterval);
    }

    if (m_score > m_highestScore)
        setHighestScore(m_score);
}

void GameController::checkBulletEnemyCollision()
{
    auto &bullets = m_bullet.getBulletLists();
    auto &enemies = m_enemy.getEnemyLists();

    if (bullets.empty() || enemies.empty())
        return;

    QVector<PlayerBullet *> bulletsToDestroy{};
    QVector<Enemy *> enemiesToDestroy{};

    foreach (PlayerBullet *bullet, bullets) {
        auto enemyToDestroy = std::ranges::find_if(enemies, [&](Enemy *enemy) {
            return isCollided(bullet, enemy);
        });
        if (enemyToDestroy == enemies.end())
            continue;

        bulletsToDestroy.push_back(bullet);
        enemiesToDestroy.push_back(*enemyToDestroy);
        m_audioManager.playHit();
        updateScore();
    }

    foreach (PlayerBullet *bullet, bulletsToDestroy)
        m_bullet.destroyBullet(bullet);

    foreach (Enemy *enemy, enemiesToDestroy)
        m_enemy.destroyEnemy(enemy);

    // m_audioManager.playHit();

    emit bulletsChanged();
    emit enemiesChanged();
}

void GameController::checkEnemyPlayerCollision()
{
    auto &enemies = m_enemy.getEnemyLists();

    if (enemies.empty()) {
        return;
    }

    QRectF playerRect{this->m_player->playerCurrentX(),
                      this->m_player->playerCurrentY(),
                      static_cast<qreal>(m_player->playerWidth()),
                      static_cast<qreal>(m_player->playerHeight())};

    Enemy *enemyToDestroy = nullptr;

    for (Enemy *enemy : enemies) {
        QRectF enemyRect{enemy->enemyX(),
                         enemy->enemyY(),
                         static_cast<qreal>(enemy->enemyWidth()),
                         static_cast<qreal>(enemy->enemyHeight())};

        if (enemy->enemyY() > m_windowHeight || playerRect.intersects(enemyRect)) {
            enemyToDestroy = enemy;
            m_audioManager.playGameOver();
            setGameState(GameState::GAMEOVER);
            gameReset();
            break;
        }
    }
    m_enemy.destroyEnemy(enemyToDestroy);
}

void GameController::gameReset()
{
    setScore(0);
    setLevel(1);

    // m_collisionTimer.stop();
    // m_enemyCreationTimer.stop();
    // m_playerMoveTimer.stop();
    m_moveDir = MoveDirection::NONE;

    m_gameControllerSettings.setValue("game/highestScore", m_highestScore);

    m_enemy.enemyReset();
    emit enemiesChanged();

    m_bullet.bulletReset();
    emit bulletsChanged();
}

QQmlListProperty<PlayerBullet> GameController::bullets()
{
    return QQmlListProperty<PlayerBullet>(this, &m_bullet.getBulletLists());
}

QQmlListProperty<Enemy> GameController::enemies()
{
    return QQmlListProperty<Enemy>(this, &m_enemy.getEnemyLists());
}

int GameController::score() const
{
    return m_score;
}

void GameController::setScore(int newScore)
{
    if (m_score == newScore)
        return;
    m_score = newScore;
    emit scoreChanged();
}

int GameController::highestScore() const
{
    return m_highestScore;
}

void GameController::setHighestScore(int newHighestScore)
{
    if (m_highestScore == newHighestScore)
        return;
    m_highestScore = newHighestScore;
    emit highestScoreChanged();
}

int GameController::level() const
{
    return m_level;
}

void GameController::setLevel(int newLevel)
{
    if (m_level == newLevel)
        return;
    m_level = newLevel;
    emit levelChanged();
}

GameController::GameState GameController::gameState() const
{
    return m_gameState;
}

void GameController::setGameState(GameState newGameState)
{
    if (m_gameState == newGameState)
        return;
    m_gameState = newGameState;
    emit gameStateChanged();
}
