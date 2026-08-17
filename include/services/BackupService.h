#ifndef BACKUPSERVICE_H
#define BACKUPSERVICE_H

#include <QString>

namespace FitCore {

struct BackupResult {
    bool success{false};
    QString message;
    QString backupFilePath;
};

class BackupService {
public:
    BackupService() = default;

    BackupResult createBackup(const QString& targetFolder = "backups");
    BackupResult restoreBackup(const QString& backupFilePath);
};

} // namespace FitCore

#endif // BACKUPSERVICE_H
