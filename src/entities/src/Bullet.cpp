#include "Bullet.h"

Bullet::Bullet(double startX, double startY, int width, int height, QObject *parent)
    : QObject{parent}
    , m_bulletX(startX)
    , m_bulletY(startY)
    , m_bulletWidth(width)
    , m_bulletHeight(height)
{}

double Bullet::bulletX() const
{
    return m_bulletX;
}

void Bullet::setBulletX(double newBulletX)
{
    if (qFuzzyCompare(m_bulletX, newBulletX))
        return;
    m_bulletX = newBulletX;
    emit bulletXChanged();
}

double Bullet::bulletY() const
{
    return m_bulletY;
}

void Bullet::setBulletY(double newBulletY)
{
    if (qFuzzyCompare(m_bulletY, newBulletY))
        return;
    m_bulletY = newBulletY;
    emit bulletYChanged();
}

int Bullet::bulletWidth() const
{
    return m_bulletWidth;
}

void Bullet::setBulletWidth(int newBulletWidth)
{
    if (m_bulletWidth == newBulletWidth)
        return;
    m_bulletWidth = newBulletWidth;
    emit bulletWidthChanged();
}

int Bullet::bulletHeight() const
{
    return m_bulletHeight;
}

void Bullet::setBulletHeight(int newBulletHeight)
{
    if (m_bulletHeight == newBulletHeight)
        return;
    m_bulletHeight = newBulletHeight;
    emit bulletHeightChanged();
}
