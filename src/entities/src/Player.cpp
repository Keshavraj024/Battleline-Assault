#include "Player.h"

Player::Player(double startX, double startY, int width, int height, QObject *parent)
    : m_playerCurrentX(startX)
    , m_playerCurrentY(startY)
    , m_playerWidth(width)
    , m_playerHeight(height)
    , QObject{parent}
{

}

double Player::playerCurrentX() const
{
    return m_playerCurrentX;
}

void Player::setPlayerCurrentX(double newPlayerCurrentX)
{
    if (qFuzzyCompare(m_playerCurrentX, newPlayerCurrentX))
        return;
    m_playerCurrentX = newPlayerCurrentX;
    emit playerCurrentXChanged();
}

double Player::playerCurrentY() const
{
    return m_playerCurrentY;
}

void Player::setPlayerCurrentY(double newPlayerCurrentY)
{
    if (qFuzzyCompare(m_playerCurrentY, newPlayerCurrentY))
        return;
    m_playerCurrentY = newPlayerCurrentY;
    emit playerCurrentYChanged();
}

int Player::playerWidth() const
{
    return m_playerWidth;
}

void Player::setPlayerWidth(int newPlayerWidth)
{
    if (m_playerWidth == newPlayerWidth)
        return;
    m_playerWidth = newPlayerWidth;
    emit playerWidthChanged();
}

int Player::playerHeight() const
{
    return m_playerHeight;
}

void Player::setPlayerHeight(int newPlayerHeight)
{
    if (m_playerHeight == newPlayerHeight)
        return;
    m_playerHeight = newPlayerHeight;
    emit playerHeightChanged();
}
