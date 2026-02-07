#include "Bullet.h"

Bullet::Bullet(QObject *parent)
    : QObject{parent}
{
    initialize();
    connect(&m_timer, &QTimer::timeout, this, &Bullet::updateBulletPos);
    m_timer.start(16);
}

void Bullet::initialize()
{
    double playerWidth = m_gameControllerSettings.getValue("player/width").toInt();

    double playerStartX = m_gameControllerSettings.getValue("player/startX").toDouble();
    double playerStartY = m_gameControllerSettings.getValue("player/startY").toDouble();

    m_bulletWidth = m_gameControllerSettings.getValue("playerBullet/width").toDouble();
    m_bulletHeight = m_gameControllerSettings.getValue("playerBullet/height").toDouble();

    m_bulletX = (playerStartX + playerWidth / 2) - (m_bulletWidth / 2);
    m_bulletY = playerStartY - m_bulletHeight;
}

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

void Bullet::updateBulletPos()
{
    setBulletY(m_bulletY - m_ySpeed);
    if(m_bulletY < -m_bulletHeight)
        emit bulletOutofWindow(this);
}

void Bullet::destroyBullet(Bullet *bulletToDestroy)
{
    auto index = m_bulletLists.indexOf(bulletToDestroy);
    if (index != -1) {
        delete m_bulletLists[index];
        m_bulletLists.removeAt(index);
        emit bulletListsChanged();
    }
}

QList<Bullet *> &Bullet::getBulletLists()
{
    return m_bulletLists;
}

void Bullet::addBullet()
{
    Bullet *newBullet = new Bullet();
    connect(newBullet, &Bullet::bulletOutofWindow, this, &Bullet::destroyBullet);
    m_bulletLists.append(newBullet);
}

bool Bullet::IsEmpty() const
{
    return m_bulletLists.empty();
}

void Bullet::clearBulletLists()
{
    m_bulletLists.clear();
}

size_t Bullet::bulletListsSize() const
{
    return m_bulletLists.size();
}

void Bullet::bulletReset()
{
    if (!IsEmpty()) {
        foreach (Bullet *bullet, m_bulletLists) {
            delete bullet;
        }
        clearBulletLists();
    }
}

void Bullet::stopBulletFallTimer()
{
    for (auto &bullet : m_bulletLists)
        bullet->m_timer.stop();
}

void Bullet::resumeBulletFallTimer()
{
    for (auto &bullet : m_bulletLists)
        bullet->m_timer.start();
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
