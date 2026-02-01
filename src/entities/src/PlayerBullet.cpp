#include "PlayerBullet.h"

PlayerBullet::PlayerBullet(QObject *parent)
    : QObject{parent}
{
    initialize();
    connect(&m_timer, &QTimer::timeout, this, &PlayerBullet::updateBulletPos);
    m_timer.start(16);
}

void PlayerBullet::initialize()
{
    double playerWidth = m_gameControllerSettings.getValue("player/width").toInt();

    double playerStartX = m_gameControllerSettings.getValue("player/startX").toDouble();
    double playerStartY = m_gameControllerSettings.getValue("player/startY").toDouble();

    m_bulletWidth = m_gameControllerSettings.getValue("playerBullet/width").toDouble();
    m_bulletHeight = m_gameControllerSettings.getValue("playerBullet/height").toDouble();

    m_bulletX = (playerStartX + playerWidth / 2) - (m_bulletWidth / 2);
    m_bulletY = playerStartY - m_bulletHeight;
}

double PlayerBullet::bulletX() const
{
    return m_bulletX;
}

void PlayerBullet::setBulletX(double newBulletX)
{
    if (qFuzzyCompare(m_bulletX, newBulletX))
        return;
    m_bulletX = newBulletX;
    emit bulletXChanged();
}

double PlayerBullet::bulletY() const
{
    return m_bulletY;
}

void PlayerBullet::setBulletY(double newBulletY)
{
    if (qFuzzyCompare(m_bulletY, newBulletY))
        return;
    m_bulletY = newBulletY;
    emit bulletYChanged();
}

void PlayerBullet::updateBulletPos()
{
    setBulletY(m_bulletY - m_ySpeed);
    if(m_bulletY < -m_bulletHeight)
        emit bulletOutofWindow(this);
}

void PlayerBullet::destroyBullet(PlayerBullet *bulletToDestroy)
{
    auto index = m_bulletLists.indexOf(bulletToDestroy);
    if (index != -1) {
        delete m_bulletLists[index];
        m_bulletLists.removeAt(index);
        emit bulletListsChanged();
    }
}

QList<PlayerBullet *> &PlayerBullet::getBulletLists()
{
    return m_bulletLists;
}

void PlayerBullet::addBullet()
{
    PlayerBullet *newBullet = new PlayerBullet();
    connect(newBullet, &PlayerBullet::bulletOutofWindow, this, &PlayerBullet::destroyBullet);
    m_bulletLists.append(newBullet);
}

bool PlayerBullet::IsEmpty() const
{
    return m_bulletLists.empty();
}

void PlayerBullet::clearBulletLists()
{
    m_bulletLists.clear();
}

size_t PlayerBullet::bulletListsSize() const
{
    return m_bulletLists.size();
}

void PlayerBullet::bulletReset()
{
    if (!IsEmpty()) {
        foreach (PlayerBullet *bullet, m_bulletLists) {
            delete bullet;
        }
        clearBulletLists();
    }
}

int PlayerBullet::bulletWidth() const
{
    return m_bulletWidth;
}

void PlayerBullet::setBulletWidth(int newBulletWidth)
{
    if (m_bulletWidth == newBulletWidth)
        return;
    m_bulletWidth = newBulletWidth;
    emit bulletWidthChanged();
}

int PlayerBullet::bulletHeight() const
{
    return m_bulletHeight;
}

void PlayerBullet::setBulletHeight(int newBulletHeight)
{
    if (m_bulletHeight == newBulletHeight)
        return;
    m_bulletHeight = newBulletHeight;
    emit bulletHeightChanged();
}
