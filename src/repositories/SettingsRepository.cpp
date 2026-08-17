#include "repositories/SettingsRepository.h"
#include "database/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

namespace FitCore {

QString SettingsRepository::get(const QString& key, const QString& defaultValue) {
    QSqlQuery query = DatabaseManager::instance().prepare("SELECT setting_value FROM settings WHERE setting_key = ?;");
    query.addBindValue(key);
    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }
    return defaultValue;
}

bool SettingsRepository::set(const QString& key, const QString& value, const QString& description) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "INSERT INTO settings (setting_key, setting_value, description) VALUES (?, ?, ?) "
        "ON CONFLICT(setting_key) DO UPDATE SET setting_value = excluded.setting_value;"
    );
    query.addBindValue(key);
    query.addBindValue(value);
    query.addBindValue(description);
    return query.exec();
}

std::map<QString, QString> SettingsRepository::getAll() {
    std::map<QString, QString> map;
    QSqlQuery query("SELECT setting_key, setting_value FROM settings;", DatabaseManager::instance().getDatabase());
    while (query.next()) {
        map[query.value("setting_key").toString()] = query.value("setting_value").toString();
    }
    return map;
}

} // namespace FitCore
