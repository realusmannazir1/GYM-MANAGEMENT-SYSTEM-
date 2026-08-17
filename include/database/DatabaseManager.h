#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <memory>
#include <mutex>

namespace FitCore {

class DatabaseManager {
public:
    static DatabaseManager& instance();

    ~DatabaseManager();

    // Prevent copy/move
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    bool initialize(const QString& dbPath = "data/fitcore.db");
    bool open();
    void close();
    bool isOpen() const;

    QSqlDatabase getDatabase();

    bool execute(const QString& sql);
    bool executeScriptFile(const QString& filePath);

    QSqlQuery prepare(const QString& sql);

    bool beginTransaction();
    bool commit();
    bool rollback();

    QString lastError() const;

private:
    DatabaseManager() = default;

    QString m_dbPath;
    QString m_connectionName;
    mutable std::recursive_mutex m_mutex;
    QString m_lastError;
};

} // namespace FitCore

#endif // DATABASEMANAGER_H
