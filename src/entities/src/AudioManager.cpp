#include "AudioManager.h"
#include <QDebug>
#include <QFile>

AudioManager::AudioManager(QObject *parent)
    : QObject(parent)
{
    m_hitSound.setSource(QUrl::fromLocalFile(":/qml/Assets/Sounds/hit.wav"));
    m_hitSound.setVolume(0.8f);

    m_gameOverSound.setSource(QUrl::fromLocalFile(":/qml/Assets/Sounds/game_over.wav"));
    m_gameOverSound.setVolume(0.9f);

    m_shootSound.setSource(QUrl::fromLocalFile(":/qml/Assets/Sounds/shoot.wav"));
    m_shootSound.setVolume(1.0f);

    m_clickSound.setSource(QUrl::fromLocalFile(":/qml/Assets/Sounds/click.wav"));
    m_clickSound.setVolume(1.0f);
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

void AudioManager::playClick()
{
    play(m_clickSound);
}

void AudioManager::setMuted(bool muted)
{
    m_muted = muted;
}

bool AudioManager::isMuted() const
{
    return m_muted;
}
