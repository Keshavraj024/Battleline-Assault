#include "PlayerBullet.h"

PlayerBullet::PlayerBullet(SettingsManager& settings,QObject *parent)
    : QObject{parent}
{
    initialize(settings);
    connect(&m_timer, &QTimer::timeout, this, &PlayerBullet::updateBulletPos);
    m_timer.start(16);
}

void PlayerBullet::initialize(SettingsManager& settings)
{
    double playerWidth = settings.getValue("player/width").toInt();

    double playerStartX = settings.getValue("player/startX").toDouble();
    double playerStartY = settings.getValue("player/startY").toDouble();

    m_bulletWidth = settings.getValue("playerBullet/width").toDouble();
    m_bulletHeight = settings.getValue("playerBullet/height").toDouble();

    m_bulletX = (playerStartX + playerWidth / 2) - (m_bulletWidth / 2);
    m_bulletY = playerStartY - m_bulletHeight;

    // qDebug() <<  m_bulletX << m_bulletY << playerStartX << playerStartY;
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
