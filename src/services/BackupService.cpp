#include "services/BackupService.h"
#include "database/DatabaseManager.h"
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QSqlQuery>
#include <QDebug>

namespace FitCore {

BackupResult BackupService::createBackup(const QString& targetFolder) {
    BackupResult res;
    QDir dir(targetFolder);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss");
    QString backupFileName = QString("FitCore_Backup_%1.db").arg(timestamp);
    QString backupPath = dir.filePath(backupFileName);

    QSqlDatabase db = DatabaseManager::instance().getDatabase();
    QSqlQuery query(db);

    // Use SQLite VACUUM INTO command for live hot backup safely
    QString sql = QString("VACUUM INTO '%1';").arg(backupPath);

    if (query.exec(sql)) {
        res.success = true;
        res.backupFilePath = backupPath;
        res.message = "Database backup created successfully at: " + backupPath;
        qInfo() << res.message;
        return res;
    }

    // Fallback: File Copy
    DatabaseManager::instance().close();
    if (QFile::copy("data/fitcore.db", backupPath)) {
        DatabaseManager::instance().open();
        res.success = true;
        res.backupFilePath = backupPath;
        res.message = "Database file backup created successfully at: " + backupPath;
        return res;
    }

    DatabaseManager::instance().open();
    res.message = "Backup failed: " + query.lastError().text();
    return res;
}

BackupResult BackupService::restoreBackup(const QString& backupFilePath) {
    BackupResult res;
    QFile file(backupFilePath);
    if (!file.exists()) {
        res.message = "Selected backup file does not exist: " + backupFilePath;
        return res;
    }

    // 1. Create emergency backup of current state
    createBackup("backups/emergency");

    // 2. Close active DB connection
    DatabaseManager::instance().close();

    // 3. Replace active database
    QFile::remove("data/fitcore.db");
    if (QFile::copy(backupFilePath, "data/fitcore.db")) {
        if (DatabaseManager::instance().open()) {
            res.success = true;
            res.message = "Database restored successfully from: " + backupFilePath;
            return res;
        }
    }

    // Rollback if restore fails
    res.message = "Restore operation failed. Please check file permissions.";
    DatabaseManager::instance().open();
    return res;
}

} // namespace FitCore
