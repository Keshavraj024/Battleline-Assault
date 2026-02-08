#include "Player.h"

Player::Player(QObject *parent)
    : QObject{parent}
{
    initialize();

    m_playerWidth = m_gameControllerSettings.getValue("Player/width").toInt();
    m_playerHeight = m_gameControllerSettings.getValue("Player/height").toInt();

    m_windowWidth = m_gameControllerSettings.getValue("Window/width").toInt();
    m_windowHeight = m_gameControllerSettings.getValue("Window/height").toInt();
}

void Player::initialize()
{
    setPlayerCurrentX(m_windowWidth / 2 - m_playerWidth / 2);
    setPlayerCurrentY(m_windowHeight - m_playerHeight);
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
