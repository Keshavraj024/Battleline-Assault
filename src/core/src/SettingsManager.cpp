#include "SettingsManager.h"

SettingsManager::SettingsManager() {
    auto setDefault = [&](const std::string &key, const QVariant &value) {
        if (!m_settings.contains(key)) {
            m_settings.setValue(key, value);
        }
    };

    setDefault("window/width", 1280);
    setDefault("window/height", 720);

    setDefault("player/width", 100);
    setDefault("player/height", 100);

    setDefault("enemy/width", 70);
    setDefault("enemy/height", 70);
    setDefault("enemy/startY", -50);

    setDefault("playerBullet/width", 10);
    setDefault("playerBullet/height", 50);

    setDefault("enemy/count", 12);

    setDefault("game/highestScore", 0);

    setDefault("audio/globalVolume", 0.6f);
    setDefault("audio/hitVolume", 0.6f);
    setDefault("audio/sho0tVolume", 0.6f);
    setDefault("audio/gameOverVolume", 0.6f);
}

SettingsManager &SettingsManager::instance()
{
    static SettingsManager settings;
    return settings;
}

void SettingsManager::setValue(const QString &key, const QVariant &val)
{
    m_settings.setValue(key, val);
}

QVariant SettingsManager::getValue(const QString &key)
{
    return  m_settings.value(key);
}

