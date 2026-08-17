#include "models/Notification.h"

namespace FitCore {

Notification::Notification()
    : m_id(0), m_userId(0), m_type("Info"), m_isRead(false) {}

int Notification::getId() const { return m_id; }
void Notification::setId(int id) { m_id = id; }

int Notification::getUserId() const { return m_userId; }
void Notification::setUserId(int userId) { m_userId = userId; }

QString Notification::getTitle() const { return m_title; }
void Notification::setTitle(const QString& title) { m_title = title; }

QString Notification::getMessage() const { return m_message; }
void Notification::setMessage(const QString& message) { m_message = message; }

QString Notification::getType() const { return m_type; }
void Notification::setType(const QString& type) { m_type = type; }

QString Notification::getCreatedAt() const { return m_createdAt; }
void Notification::setCreatedAt(const QString& createdAt) { m_createdAt = createdAt; }

QString Notification::getReadAt() const { return m_readAt; }
void Notification::setReadAt(const QString& readAt) { m_readAt = readAt; }

bool Notification::isRead() const { return m_isRead; }
void Notification::setRead(bool isRead) { m_isRead = isRead; }

} // namespace FitCore
