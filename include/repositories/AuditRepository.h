#ifndef AUDITREPOSITORY_H
#define AUDITREPOSITORY_H

#include "models/AuditLog.h"
#include <vector>

namespace FitCore {

class AuditRepository {
public:
    AuditRepository() = default;

    std::vector<AuditLog> findAllPaged(int limit, int offset);
    bool logAction(int userId, const QString& action, const QString& entityName, int entityId, const QString& description);
};

} // namespace FitCore

#endif // AUDITREPOSITORY_H
