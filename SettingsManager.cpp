#include "SettingsManager.h"

SettingsManager::SettingsManager() {
    setValue("window/width", 1280);
    setValue("window/height", 720);

    setValue("player/width", 100);
    setValue("player/height", 100);

    setValue("enemy/width", 70);
    setValue("enemy/height", 70);
    setValue("enemy/startY", -50);

    setValue("playerBullet/width", 10);
    setValue("playerBullet/height", 50);

    setValue("enemy/count", 12);

    setValue("game/highestScore", 0);
}

void SettingsManager::setValue(const QString &key, const QVariant &val)
{
    m_settings.setValue(key, val);
}

QVariant SettingsManager::getValue(const QString &key)
{
    return  m_settings.value(key);
}

