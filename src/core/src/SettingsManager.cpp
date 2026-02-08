#include "SettingsManager.h"

SettingsManager::SettingsManager() {
    auto setDefault = [&](const std::string &key, const QVariant &value) {
        if (!m_settings.contains(key)) {
            m_settings.setValue(key, value);
        }
    };

    setDefault("Window/width", 1280);
    setDefault("Window/height", 720);

    setDefault("Player/width", 100);
    setDefault("Player/height", 100);

    setDefault("Enemy/width", 70);
    setDefault("Enemy/height", 70);

    setDefault("Bullet/width", 10);
    setDefault("Bullet/height", 50);

    setDefault("Game/highestScore", 0);

    setDefault("Audio/globalVolume", 0.6f);
    setDefault("Audio/hitVolume", 0.6f);
    setDefault("Audio/shootVolume", 0.6f);
    setDefault("Audio/gameOverVolume", 0.6f);
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

