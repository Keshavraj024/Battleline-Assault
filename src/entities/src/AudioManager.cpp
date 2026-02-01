#include "AudioManager.h"
#include <QDebug>
#include <QFile>

AudioManager::AudioManager(QObject *parent)
    : QObject(parent)
{
    m_hitSound.setSource(
        QUrl::fromLocalFile(":/qt/qml/Battleline_Assault/qml/Assets/Sounds/hit.wav"));
    m_hitSound.setVolume(0.8f);

    m_gameOverSound.setSource(
        QUrl::fromLocalFile(":/qt/qml/Battleline_Assault/qml/Assets/Sounds/game_over.wav"));
    m_gameOverSound.setVolume(0.9f);

    m_shootSound.setSource(
        QUrl::fromLocalFile(":/qt/qml/Battleline_Assault/qml/Assets/Sounds/shoot.wav"));
    m_shootSound.setVolume(1.0f);
}

void AudioManager::play(QSoundEffect &effect)
{
    if (m_muted)
        return;

    effect.stop();
    effect.play();
}

void AudioManager::playHit()
{
    play(m_hitSound);
}

void AudioManager::playGameOver()
{
    play(m_gameOverSound);
}

void AudioManager::playShoot()
{
    play(m_shootSound);
}

void AudioManager::setMuted(bool muted)
{
    m_muted = muted;
}

bool AudioManager::isMuted() const
{
    return m_muted;
}
