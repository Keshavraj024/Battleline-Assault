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
constexpr int FRAME_COUNT_PER_SEC = 60;

GameController::GameController(QObject *parent)
    : QObject{parent}
{
    m_windowWidth = m_gameControllerSettings.getValue("Window/width").toInt();
    m_windowHeight = m_gameControllerSettings.getValue("Window/height").toInt();

    m_highestScore = m_gameControllerSettings.getValue("Game/highestScore").toInt();

    m_bulletWidth = m_gameControllerSettings.getValue("Bullet/width").toInt();
    m_bulletHeight = m_gameControllerSettings.getValue("Bullet/height").toInt();

    m_enemyWidth = m_gameControllerSettings.getValue("Enemy/width").toInt();
    m_enemyHeight = m_gameControllerSettings.getValue("Enemy/height").toInt();

    m_playerWidth = m_gameControllerSettings.getValue("Player/width").toInt();
    m_playerHeight = m_gameControllerSettings.getValue("Player/height").toInt();

    connect(&m_thrustTimer, &QTimer::timeout, this, &GameController::applyGravity);
    m_thrustTimer.setInterval(16);

    connect(&m_gameTimer, &QTimer::timeout, this, &GameController::gameTick);
    m_gameTimer.setInterval(16);

    m_enemyManager = new EnemyManager(this);
    m_bulletManager = new BulletManager(this);

    m_player = new Player(0, 0, m_playerWidth, m_playerHeight, this);
    updatePlayerStartingPosition();
}

void GameController::initialize()
{
    updatePlayerStartingPosition();

    m_gameTimer.start();

    m_elapsedTimer.start();

    if (!m_frameTimer.isValid()) {
        m_frameTimer.start();
    }

    setScore(0);
    setLevel(1);

    m_bulletCreationTimer.setSingleShot(true);
}

void GameController::updatePlayerStartingPosition()
{
    if (!m_player)
        return;

    float playerStartX = m_windowWidth / 2 - m_playerWidth / 2;
    float playerStartY = m_windowHeight - m_playerHeight;

    m_player->setPlayerCurrentX(playerStartX);
    m_player->setPlayerCurrentY(playerStartY);
}

void GameController::gameTick()
{
    float deltaTime = 0.0f;

    // Initialize timer if invalid, but DON'T return
    if (!m_frameTimer.isValid()) {
        m_frameTimer.start();
        // deltaTime stays 0.0f for the very first frame
    } else {
        deltaTime = std::min(m_frameTimer.restart() / 1000.0f, 0.03f);
    }

    checkCollision();
    updatePlayerMovement();

    if (m_KeyUpPressed) {
        applyBoost();
    }

    m_timeSinceLastSpawn += deltaTime;

    float spawnThreshold = std::max(MIN_INTERVAL, BASE_INTERVAL - (DECREASE_PER_LEVEL * m_level))
                           / 1000.f;

    if (m_timeSinceLastSpawn >= spawnThreshold) {
        m_enemyManager->spawnEnemy(m_enemyWidth, m_enemyHeight, m_windowWidth);
        m_timeSinceLastSpawn = 0.0f;
    }
}

EnemyManager *GameController::enemyManager() const
{
    return m_enemyManager;
}

BulletManager *GameController::bulletManager() const
{
    return m_bulletManager;
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

void GameController::togglePause()
{
    if (m_gameState == GameState::RUNNING) {
        m_gameTimer.stop();

        m_enemyManager->togglePause(true);
        m_bulletManager->togglePause(true);
        setGameState(GameState::PAUSED);

    } else if (m_gameState == GameState::PAUSED) {
        m_gameTimer.start();
        m_enemyManager->togglePause(false);
        m_bulletManager->togglePause(false);

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

void GameController::setPressed(bool pressed)
{
    m_KeyUpPressed = pressed;

    if (m_KeyUpPressed && !m_thrustTimer.isActive()) {
        m_thrustTimer.start();
    }
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
    qDebug() << "Applying boost";
    m_playerYOffset = -10;
    if (!m_thrustTimer.isActive())
        m_thrustTimer.start(); // 60 FPS
}

void GameController::shootBullet()
{
    if (!m_bulletCreationTimer.isActive()) {
        m_audioManager.playShoot();

        double px = m_player->playerCurrentX();
        double py = m_player->playerCurrentY();
        double pw = m_player->playerWidth();

        double startX = (px + pw / 2.0) - (m_bulletWidth / 2.0);
        double startY = py - m_bulletHeight;

        m_bulletManager->spawnBullet(startX, startY, m_bulletWidth, m_bulletHeight);
        m_bulletCreationTimer.start(50);
    }
}

void GameController::applyGravity()
{
    double newYPos = m_player->playerCurrentY() + m_playerYOffset;

    if (newYPos > (m_windowHeight - m_playerHeight)) {
        newYPos = m_windowHeight - m_playerHeight;
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

    newPos = std::clamp(newPos, m_playerminX, m_windowWidth - m_player->playerWidth());
    m_player->setPlayerCurrentX(newPos);
}

void GameController::checkCollision()
{
    checkBulletEnemyCollision();
    checkEnemyPlayerCollision();
}

bool isCollided(Bullet *bullet, Enemy *enemy)
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
    }

    if (m_score > m_highestScore)
        setHighestScore(m_score);
}

void GameController::checkBulletEnemyCollision()
{
    auto &bullets = m_bulletManager->getBulletsList();
    auto &enemies = m_enemyManager->getEnemiesList();

    if (bullets.empty() || enemies.empty())
        return;

    QVector<int> enemyindicesToRemove{};
    QVector<int> bulletIndicesToRemove{};

    for (int bulletIdx = 0; bulletIdx < bullets.size(); bulletIdx++) {
        for (int enemyIdx = 0; enemyIdx < enemies.size(); enemyIdx++) {
            if (isCollided(bullets[bulletIdx], enemies[enemyIdx])) {
                if (!enemyindicesToRemove.contains(enemyIdx))
                    enemyindicesToRemove.push_back(enemyIdx);

                if (!bulletIndicesToRemove.contains(bulletIdx))
                    bulletIndicesToRemove.push_back(bulletIdx);

                m_audioManager.playHit();
                updateScore();
            }
        }
    }

    std::ranges::sort(enemyindicesToRemove, std::greater<int>());
    foreach (int enemyIdx, enemyindicesToRemove)
        m_enemyManager->removeEnemy(enemyIdx);

    std::ranges::sort(bulletIndicesToRemove, std::greater<int>());
    foreach (int bulletIdx, bulletIndicesToRemove)
        m_bulletManager->removeBullet(bulletIdx);
}

void GameController::checkEnemyPlayerCollision()
{
    auto &enemies = m_enemyManager->getEnemiesList();

    if (enemies.empty()) {
        return;
    }

    QRectF playerRect{this->m_player->playerCurrentX(),
                      this->m_player->playerCurrentY(),
                      static_cast<qreal>(m_player->playerWidth()),
                      static_cast<qreal>(m_player->playerHeight())};

    for (size_t enemyIdx = 0; enemyIdx < enemies.size(); enemyIdx++) {
        QRectF enemyRect{enemies[enemyIdx]->enemyX(),
                         enemies[enemyIdx]->enemyY(),
                         static_cast<qreal>(enemies[enemyIdx]->enemyWidth()),
                         static_cast<qreal>(enemies[enemyIdx]->enemyHeight())};

        if (enemies[enemyIdx]->enemyY() > m_windowHeight || playerRect.intersects(enemyRect)) {
            m_audioManager.playGameOver();
            setGameState(GameState::GAMEOVER);
            gameReset();
            break;
        }
    }
}

void GameController::gameReset()
{
    m_gameTimer.stop();

    m_moveDir = MoveDirection::NONE;

    m_gameControllerSettings.setValue("Game/highestScore", m_highestScore);

    m_enemyManager->clearEnemies();
    m_bulletManager->clearBullets();
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

Player *GameController::player() const
{
    return m_player;
}
