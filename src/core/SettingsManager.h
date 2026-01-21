#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QSettings>
#include <QDebug>
#include <QVariant>
#include <QString>

class SettingsManager
{
public:
    SettingsManager();

    void setValue(const QString& key, const QVariant& val);
    QVariant getValue(const QString& key);

private:
    QSettings m_settings{"GameSettings.ini", QSettings::Format::IniFormat};
};

#endif // SETTINGSMANAGER_H
