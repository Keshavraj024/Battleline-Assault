#include "GameController.h"
#include <QRectF>
#include <QRandomGenerator>

GameController::GameController(SettingsManager& settings, Player *player, QObject *parent)
    : QObject{parent}, m_gameControllerSettings(settings), m_player(player)
{
    initialize();
    connect(&m_thrustTimer, &QTimer::timeout, this, &GameController::applyGravity);

    connect(&m_enemyCreationTimer, &QTimer::timeout, this, &GameController::createEnemies);

    connect(&m_collisionTimer, &QTimer::timeout, this, &GameController::checkCollision);

    connect(&m_playerMoveTimer, &QTimer::timeout, this, &GameController::updatePlayerMovement);
}


void GameController::initialize()
{
    m_collisionTimer.start(16);
    m_enemyCreationTimer.start(2000);

    if(m_playerMoveTimer.isActive())
        m_playerMoveTimer.stop();

    setScore(0);

    m_windowWidth = m_gameControllerSettings.getValue("window/width").toInt();
    m_windowHeight = m_gameControllerSettings.getValue("window/height").toInt();

    m_highestScore = m_gameControllerSettings.getValue("game/highestScore").toInt();

    m_gameControllerSettings.setValue("player/startX", m_player->playerCurrentX());
    m_gameControllerSettings.setValue("player/startY", m_player->playerCurrentY());

}


void GameController::moveLeft()
{
    m_moveDir = MoveDirection::LEFT;
    if(!m_playerMoveTimer.isActive())
        m_playerMoveTimer.start(16);

}

void GameController::moveRight()
{
    m_moveDir = MoveDirection::RIGHT;
    if(!m_playerMoveTimer.isActive())
        m_playerMoveTimer.start(16);

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
    if(!m_thrustTimer.isActive())
        m_thrustTimer.start(16); // 60 FPS
}

void GameController::shootBullet()
{
    m_gameControllerSettings.setValue("player/startX", m_player->playerCurrentX());
    m_gameControllerSettings.setValue("player/startY", m_player->playerCurrentY());

    PlayerBullet* newBullet = new PlayerBullet(m_gameControllerSettings);
    connect(newBullet, &PlayerBullet::bulletOutofWindow, this, &GameController::destroyBullet);
    m_bulletList.append(newBullet);

    emit bulletsChanged();
}

void GameController::createEnemies()
{
    quint32 enemyStartX = QRandomGenerator::global()->bounded(50, m_windowWidth - 50);

    m_gameControllerSettings.setValue("enemy/startX", enemyStartX);

    m_enemyList.append(new Enemy(m_gameControllerSettings));

    emit enemiesChanged();
}

void GameController::stopPlayerMoveTimer()
{
    m_moveDir = MoveDirection::NONE;
    if(m_playerMoveTimer.isActive())
        m_playerMoveTimer.stop();
}

void GameController::restartGame()
{
    initialize();
}

void GameController::applyGravity()
{
    double newYPos = m_player->playerCurrentY() + m_playerYOffset;

    if(newYPos > (m_windowHeight - m_player->playerHeight())){
        newYPos = m_windowHeight - m_player->playerHeight();
        if(m_thrustTimer.isActive())
            m_thrustTimer.stop();
    }

    if(newYPos < m_windowHeight / 2)
        newYPos = m_windowHeight / 2;

    m_player->setPlayerCurrentY(newYPos);
    m_playerYOffset += m_gravity;
}

void GameController::destroyBullet(PlayerBullet* bulletToDestroy)
{
    auto index = m_bulletList.indexOf(bulletToDestroy);
    if(index != -1) {
        delete m_bulletList[index];
        m_bulletList.removeAt(index);
        emit bulletsChanged();
    }
}

void GameController::destroyEnemy(Enemy* enemyToDestroy)
{
    auto index = m_enemyList.indexOf(enemyToDestroy);
    if(index != -1) {
        delete m_enemyList[index];
        m_enemyList.removeAt(index);
        emit enemiesChanged();
    }
}

void GameController::updatePlayerMovement()
{
    if(m_moveDir == MoveDirection::LEFT && m_player->playerCurrentX() > m_minX)
        m_player->setPlayerCurrentX(m_player->playerCurrentX() - m_playerXOffset);
    else if(m_moveDir == MoveDirection::RIGHT && m_player->playerCurrentX() < (m_windowWidth - m_player->playerWidth()))
        m_player->setPlayerCurrentX(m_player->playerCurrentX() + m_playerXOffset);
}

bool isCollided( PlayerBullet* bullet,  Enemy* enemy) {
    QRectF bulletRect {bullet->bulletX(), bullet->bulletY(), static_cast<qreal>(bullet->bulletWidth()), static_cast<qreal>(bullet->bulletHeight())};
    QRectF enemyRect {enemy->enemyX(), enemy->enemyY(), static_cast<qreal>(enemy->enemyWidth()), static_cast<qreal>(enemy->enemyHeight())};

    if(bulletRect.intersects(enemyRect)) {
        // qDebug() << "Collsion occured ";
        return true;
    }
    return false;
}

void GameController::checkCollision()
{
    if(m_bulletList.size() > 0 && m_enemyList.size() > 0) {
        for(int bullet = m_bulletList.size() - 1; bullet >= 0; --bullet){
            // qDebug() << "m_bulletList.size()" << bullet;
            for(int enemy = m_enemyList.size() - 1; enemy >= 0; --enemy) {

                if(isCollided(m_bulletList[bullet], m_enemyList[enemy])){
                    destroyBullet(m_bulletList[bullet]);
                    destroyEnemy(m_enemyList[enemy]);
                    setScore(m_score + 10);
                    if(m_score > m_highestScore)
                        setHighestScore(m_score);
                    break;
                }
            }
        }
    }
    checkEnemyPlayerCollision();
}

void GameController::checkEnemyPlayerCollision()
{
    foreach(Enemy* enemy, m_enemyList) {
        QRectF playerRect {this->m_player->playerCurrentX(), this->m_player->playerCurrentY(), static_cast<qreal>(m_player->playerWidth()), static_cast<qreal>(m_player->playerHeight())};
        QRectF enemyRect {enemy->enemyX(), enemy->enemyY(), static_cast<qreal>(enemy->enemyWidth()), static_cast<qreal>(enemy->enemyHeight())};

        if(enemy->enemyY() > m_windowHeight || playerRect.intersects(enemyRect)) {
            destroyEnemy(enemy);
            emit gameOver();
            gameReset();
            break;
        }
    }
}

void GameController::gameReset()
{
    m_collisionTimer.stop();
    m_enemyCreationTimer.stop();

    m_gameControllerSettings.setValue("game/highestScore", m_highestScore);

    if(!m_enemyList.isEmpty()){
        foreach (Enemy* enemy, m_enemyList) {
            delete enemy;
        }
        m_enemyList.clear();
        emit enemiesChanged();
    }

    if(!m_bulletList.isEmpty()){
        foreach (PlayerBullet* bullet, m_bulletList) {
            delete bullet;
        }
        m_bulletList.clear();
        emit bulletsChanged();
    }
}

QQmlListProperty<PlayerBullet> GameController::bullets()
{
    return QQmlListProperty<PlayerBullet>(this, &m_bulletList);
}

QQmlListProperty<Enemy> GameController::enemies()
{
    return QQmlListProperty<Enemy>(this, &m_enemyList);
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
