#include "repositories/AuditRepository.h"
#include "database/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

namespace FitCore {

std::vector<AuditLog> AuditRepository::findAllPaged(int limit, int offset) {
    std::vector<AuditLog> list;
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT a.*, u.username "
        "FROM audit_logs a LEFT JOIN users u ON a.user_id = u.user_id "
        "ORDER BY a.audit_id DESC LIMIT ? OFFSET ?;"
    );
    query.addBindValue(limit);
    query.addBindValue(offset);

    if (query.exec()) {
        while (query.next()) {
            AuditLog log;
            log.setId(query.value("audit_id").toInt());
            log.setUserId(query.value("user_id").toInt());
            log.setUsername(query.value("username").toString());
            log.setAction(query.value("action").toString());
            log.setEntityName(query.value("entity_name").toString());
            log.setEntityId(query.value("entity_id").toInt());
            log.setDescription(query.value("description").toString());
            log.setTimestamp(query.value("timestamp").toString());
            list.push_back(log);
        }
    }
    return list;
}

bool AuditRepository::logAction(int userId, const QString& action, const QString& entityName, int entityId, const QString& description) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "INSERT INTO audit_logs (user_id, action, entity_name, entity_id, description) VALUES (?, ?, ?, ?, ?);"
    );
    query.addBindValue(userId > 0 ? QVariant(userId) : QVariant(QVariant::Int));
    query.addBindValue(action);
    query.addBindValue(entityName);
    query.addBindValue(entityId > 0 ? QVariant(entityId) : QVariant(QVariant::Int));
    query.addBindValue(description);

    return query.exec();
}

} // namespace FitCore
