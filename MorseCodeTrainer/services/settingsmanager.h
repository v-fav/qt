#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QString>

class SettingsManager
{
public:
    static QString language();
    static void setLanguage(const QString& language);

    static QString theme();
    static void setTheme(const QString& theme);

    static bool keyboardInputEnabled();
    static void setKeyboardInputEnabled(bool enabled);

    static bool spaceInputEnabled();
    static void setSpaceInputEnabled(bool enabled);
};

#endif
