#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QSettings>
#include <QDebug>
#include <QVariant>
#include <QString>

class SettingsManager
{
public:
    static SettingsManager &instance();

    void setValue(const QString& key, const QVariant& val);
    QVariant getValue(const QString& key);

    inline SettingsManager &Settings() { return SettingsManager::instance(); }

    SettingsManager(const SettingsManager &) = delete;
    SettingsManager &operator=(const SettingsManager &) = delete;
    SettingsManager(SettingsManager &&) = delete;
    SettingsManager &operator=(SettingsManager &&) = delete;

private:
    QSettings m_settings{"GameSettings.ini", QSettings::Format::IniFormat};

    SettingsManager();
};

#endif // SETTINGSMANAGER_H
