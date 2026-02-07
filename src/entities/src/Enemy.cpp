#include "Enemy.h"
#include <QRandomGenerator>

Enemy::Enemy(QObject *parent)
    : QObject{parent}
{
    initialize();
}

void Enemy::initialize()
{
    m_enemyWidth = m_gameControllerSettings.getValue("enemy/width").toInt();
    m_enemyHeight = m_gameControllerSettings.getValue("enemy/height").toInt();

    m_windowHeight =  m_gameControllerSettings.getValue("window/height").toInt();
    m_windowWidth = m_gameControllerSettings.getValue("window/width").toInt();
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

QString Enemy::imageSource() const
{
    return m_imageSource;
}

void Enemy::setImageSource(const QString &source)
{
    m_imageSource = source;
}
