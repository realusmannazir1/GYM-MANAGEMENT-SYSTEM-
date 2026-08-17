#ifndef SETTINGSREPOSITORY_H
#define SETTINGSREPOSITORY_H

#include <QString>
#include <map>

namespace FitCore {

class SettingsRepository {
public:
    SettingsRepository() = default;

    QString get(const QString& key, const QString& defaultValue = "");
    bool set(const QString& key, const QString& value, const QString& description = "");
    std::map<QString, QString> getAll();
};

} // namespace FitCore

#endif // SETTINGSREPOSITORY_H
