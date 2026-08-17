#ifndef AUDITLOG_H
#define AUDITLOG_H

#include <QString>

namespace FitCore {

class AuditLog {
public:
    AuditLog();

    int getId() const;
    void setId(int id);

    int getUserId() const;
    void setUserId(int userId);

    QString getUsername() const;
    void setUsername(const QString& name);

    QString getAction() const;
    void setAction(const QString& action);

    QString getEntityName() const;
    void setEntityName(const QString& entity);

    int getEntityId() const;
    void setEntityId(int entityId);

    QString getDescription() const;
    void setDescription(const QString& desc);

    QString getTimestamp() const;
    void setTimestamp(const QString& timestamp);

private:
    int m_id;
    int m_userId;
    QString m_username;
    QString m_action;
    QString m_entityName;
    int m_entityId;
    QString m_description;
    QString m_timestamp;
};

} // namespace FitCore

#endif // AUDITLOG_H
