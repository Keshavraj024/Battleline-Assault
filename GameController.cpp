#include "GameController.h"
#include <QRectF>

GameController::GameController(SettingsManager& settings,QObject *parent)
    : QObject{parent}, m_gameControllerSettings(settings)
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

    m_playerWidth = m_gameControllerSettings.getValue("player/width").toInt();
    m_playerHeight = m_gameControllerSettings.getValue("player/height").toInt();

    m_highestScore = m_gameControllerSettings.getValue("game/highestScore").toInt();

    setCurrentX(m_windowWidth / 2 - m_playerWidth / 2);
    setCurrentY(m_windowHeight - m_playerHeight);

    // qDebug() << currentX() << currentY();

    m_gameControllerSettings.setValue("player/startX", currentX());
    m_gameControllerSettings.setValue("player/startY", currentY());
}

double GameController::currentX() const
{
    return m_currentX;
}

void GameController::setCurrentX(double newCurrentX)
{
    if (qFuzzyCompare(m_currentX, newCurrentX))
        return;
    m_currentX = newCurrentX;
    emit currentXChanged();
}

double GameController::currentY() const
{
    return m_currentY;
}

void GameController::setCurrentY(double newCurrentY)
{
    if (qFuzzyCompare(m_currentY, newCurrentY))
        return;
    m_currentY = newCurrentY;
    emit currentYChanged();
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


int GameController::playerWidth() const
{
    return m_playerWidth;
}

void GameController::setPlayerWidth(int newPlayerWidth)
{
    if (m_playerWidth == newPlayerWidth)
        return;
    m_playerWidth = newPlayerWidth;
    emit playerWidthChanged();
}

int GameController::playerHeight() const
{
    return m_playerHeight;
}

void GameController::setPlayerHeight(int newPlayerHeight)
{
    if (m_playerHeight == newPlayerHeight)
        return;
    m_playerHeight = newPlayerHeight;
    emit playerHeightChanged();
}

void GameController::applyBoost()
{
    m_yOffset = -10;
    if(!m_thrustTimer.isActive())
        m_thrustTimer.start(16); // 60 FPS
}

void GameController::shootBullet()
{
    m_gameControllerSettings.setValue("player/startX", m_currentX);
    m_gameControllerSettings.setValue("player/startY", m_currentY);

    PlayerBullet* newBullet = new PlayerBullet(m_gameControllerSettings);
    connect(newBullet, &PlayerBullet::bulletOutofWindow, this, &GameController::destroyBullet);
    m_bulletList.append(newBullet);


    // qDebug() << "Creating Player bullet" << m_bulletList.size();
    emit bulletsChanged();
}

void GameController::createEnemies()
{
    m_gameControllerSettings.setValue("enemy/startX", rand() % m_windowWidth - 20);

    m_enemyList.append(new Enemy(m_gameControllerSettings));
    // qDebug() << "Creating Player bullet" << m_bulletList.size();
    // checkEnemyPlayerCollision();
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
    // emit currentXChanged();
    // emit currentYChanged();
}

void GameController::applyGravity()
{
    double newYPos = m_currentY + m_yOffset;

    if(newYPos > (m_windowHeight - m_playerHeight)){
        newYPos = m_windowHeight - m_playerHeight;
        if(m_thrustTimer.isActive())
            m_thrustTimer.stop();
    }

    if(newYPos < m_windowHeight / 2)
        newYPos = m_windowHeight / 2;

    setCurrentY(newYPos);
    m_yOffset += m_gravity;
}

void GameController::destroyBullet(PlayerBullet* bulletToDestroy)
{
    auto index = m_bulletList.indexOf(bulletToDestroy);
    if(index != -1) {
        delete m_bulletList[index];
        m_bulletList.removeAt(index);
        emit bulletsChanged();
        // qDebug() << "Bullet Destroyed ";
    }
}

void GameController::destroyEnemy(Enemy* enemyToDestroy)
{
    auto index = m_enemyList.indexOf(enemyToDestroy);
    if(index != -1) {
        delete m_enemyList[index];
        m_enemyList.removeAt(index);
        emit enemiesChanged();
        // qDebug() << "Enemy Destroyed ";
    }
}

void GameController::updatePlayerMovement()
{
    if(m_moveDir == MoveDirection::LEFT && m_currentX > m_minX)
        setCurrentX(m_currentX - m_xOffset);
    else if(m_moveDir == MoveDirection::RIGHT && m_currentX < (m_windowWidth - m_playerWidth))
        setCurrentX(m_currentX + m_xOffset);
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
        QRectF playerRect {this->m_currentX, this->m_currentY, static_cast<qreal>(this->m_playerWidth), static_cast<qreal>(this->m_playerHeight)};
        QRectF enemyRect {enemy->enemyX(), enemy->enemyY(), static_cast<qreal>(enemy->enemyWidth()), static_cast<qreal>(enemy->enemyHeight())};

        if(enemy->enemyY() > m_windowHeight || playerRect.intersects(enemyRect)) {
            destroyEnemy(enemy);
            emit gameOver();
            // gameReset();
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
