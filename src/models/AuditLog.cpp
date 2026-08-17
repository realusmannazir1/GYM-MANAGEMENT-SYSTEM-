#include "models/AuditLog.h"

namespace FitCore {

AuditLog::AuditLog()
    : m_id(0), m_userId(0), m_entityId(0) {}

int AuditLog::getId() const { return m_id; }
void AuditLog::setId(int id) { m_id = id; }

int AuditLog::getUserId() const { return m_userId; }
void AuditLog::setUserId(int userId) { m_userId = userId; }

QString AuditLog::getUsername() const { return m_username; }
void AuditLog::setUsername(const QString& name) { m_username = name; }

QString AuditLog::getAction() const { return m_action; }
void AuditLog::setAction(const QString& action) { m_action = action; }

QString AuditLog::getEntityName() const { return m_entityName; }
void AuditLog::setEntityName(const QString& entity) { m_entityName = entity; }

int AuditLog::getEntityId() const { return m_entityId; }
void AuditLog::setEntityId(int entityId) { m_entityId = entityId; }

QString AuditLog::getDescription() const { return m_description; }
void AuditLog::setDescription(const QString& desc) { m_description = desc; }

QString AuditLog::getTimestamp() const { return m_timestamp; }
void AuditLog::setTimestamp(const QString& timestamp) { m_timestamp = timestamp; }

} // namespace FitCore
