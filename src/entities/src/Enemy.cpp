#include "Enemy.h"
#include <QRandomGenerator>

Enemy::Enemy(QObject *parent)
    : QObject{parent}
{
    initialize();
    connect(&m_enemyFalltimer, &QTimer::timeout, this, &Enemy::updateEnemyPos);
    m_enemyFalltimer.start(50);
}

void Enemy::initialize()
{
    m_enemyWidth = m_gameControllerSettings.getValue("enemy/width").toInt();
    m_enemyHeight = m_gameControllerSettings.getValue("enemy/height").toInt();

    m_windowHeight =  m_gameControllerSettings.getValue("window/height").toInt();
    m_windowWidth = m_gameControllerSettings.getValue("window/width").toInt();

    m_enemyY = m_gameControllerSettings.getValue("enemy/startY").toDouble();
    m_enemyX = m_gameControllerSettings.getValue("enemy/startX").toDouble();

    int enemyCount = m_gameControllerSettings.getValue("enemy/count").toInt();
    m_enemyImageIndex = rand() % enemyCount;
}


double Enemy::enemyX() const
{
    return m_enemyX;
}

void Enemy::setEnemyX(double newEnemyX)
{
    if (qFuzzyCompare(m_enemyX, newEnemyX))
        return;
    m_enemyX = newEnemyX;
    emit enemyXChanged();
}

double Enemy::enemyY() const
{
    return m_enemyY;
}

void Enemy::setEnemyY(double newEnemyY)
{
    if (qFuzzyCompare(m_enemyY, newEnemyY))
        return;
    m_enemyY = newEnemyY;
    emit enemyYChanged();
}

int Enemy::enemyWidth() const
{
    return m_enemyWidth;
}

void Enemy::setEnemyWidth(int newEnemyWidth)
{
    if (m_enemyWidth == newEnemyWidth)
        return;
    m_enemyWidth = newEnemyWidth;
    emit enemyWidthChanged();
}

int Enemy::enemyHeight() const
{
    return m_enemyHeight;
}

void Enemy::setEnemyHeight(int newEnemyHeight)
{
    if (m_enemyHeight == newEnemyHeight)
        return;
    m_enemyHeight = newEnemyHeight;
    emit enemyHeightChanged();
}

void Enemy::updateEnemyPos()
{
    setEnemyY(m_enemyY + m_enemyFallSpeed);

    if(m_enemyY > m_windowHeight) {
        m_enemyFalltimer.stop();
    }
}

void Enemy::destroyEnemy(Enemy *enemyToDestroy)
{
    auto index = m_enemyList.indexOf(enemyToDestroy);
    if (index != -1) {
        delete m_enemyList[index];
        m_enemyList.removeAt(index);
    }
}

QList<Enemy *> &Enemy::getEnemyLists()
{
    return m_enemyList;
}

void Enemy::addEnemy()
{
    quint32 enemyStartX = QRandomGenerator::global()->bounded(50, m_windowWidth - 50);

    m_gameControllerSettings.setValue("enemy/startX", enemyStartX);

    m_enemyList.append(new Enemy());
}

bool Enemy::IsEmpty() const
{
    return m_enemyList.empty();
}

void Enemy::clearEnemyLists()
{
    m_enemyList.clear();
}

size_t Enemy::enemyListsSize() const
{
    return m_enemyList.size();
}

void Enemy::enemyReset()
{
    if (!IsEmpty()) {
        foreach (Enemy *enemy, m_enemyList) {
            delete enemy;
        }
        clearEnemyLists();
    }
}

int Enemy::enemyImageIndex() const
{
    return m_enemyImageIndex;
}

void Enemy::setEnemyImageIndex(int newEnemyImageIndex)
{
    if (m_enemyImageIndex == newEnemyImageIndex)
        return;
    m_enemyImageIndex = newEnemyImageIndex;
    emit enemyImageIndexChanged();
}
