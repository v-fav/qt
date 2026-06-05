#include "settingsmanager.h"

#include <QSettings>

QString SettingsManager::language()
{
    QSettings settings;

    return settings.value(
                       "language",
                       "English").toString();
}

void SettingsManager::setLanguage(
    const QString& language)
{
    QSettings settings;

    settings.setValue(
        "language",
        language);

    settings.sync();
}

QString SettingsManager::theme()
{
    QSettings settings;

    return settings.value(
                       "theme",
                       "Dark").toString();
}

void SettingsManager::setTheme(
    const QString& theme)
{
    QSettings settings;

    settings.setValue(
        "theme",
        theme);
}

bool SettingsManager::keyboardInputEnabled()
{
    QSettings settings;

    return settings.value(
                       "keyboardInput",
                       true).toBool();
}

void SettingsManager::setKeyboardInputEnabled(
    bool enabled)
{
    QSettings settings;

    settings.setValue(
        "keyboardInput",
        enabled);
}

bool SettingsManager::spaceInputEnabled()
{
    QSettings settings;

    return settings.value(
                       "spaceInput",
                       true).toBool();
}

void SettingsManager::setSpaceInputEnabled(
    bool enabled)
{
    QSettings settings;

    settings.setValue(
        "spaceInput",
        enabled);
}
