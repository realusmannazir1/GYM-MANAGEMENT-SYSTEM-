#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QString>

namespace FitCore {

class ThemeManager {
public:
    static QString getDarkThemeQss();
    static QString getLightThemeQss();
    static QString getBadgeStyle(const QString& status);
};

} // namespace FitCore

#endif // THEMEMANAGER_H
