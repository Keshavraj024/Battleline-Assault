#pragma once

#include <QObject>
#include <QSoundEffect>

class AudioManager : public QObject
{
    Q_OBJECT

public:
    explicit AudioManager(QObject *parent = nullptr);

    // sound effects
    void playHit();
    void playGameOver();
    void playShoot();

    // settings
    void setMuted(bool muted);
    bool isMuted() const;

private:
    void play(QSoundEffect &effect);

private:
    QSoundEffect m_hitSound;
    QSoundEffect m_gameOverSound;
    QSoundEffect m_shootSound;

    bool m_muted = false;
};
