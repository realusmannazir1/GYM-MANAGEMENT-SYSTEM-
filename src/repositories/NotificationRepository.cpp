#include "repositories/NotificationRepository.h"
#include "database/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDateTime>

namespace FitCore {

static Notification mapNotification(const QSqlQuery& query) {
    Notification n;
    n.setId(query.value("notification_id").toInt());
    n.setUserId(query.value("user_id").toInt());
    n.setTitle(query.value("title").toString());
    n.setMessage(query.value("message").toString());
    n.setType(query.value("type").toString());
    n.setCreatedAt(query.value("created_at").toString());
    n.setReadAt(query.value("read_at").toString());
    n.setRead(query.value("is_read").toInt() == 1);
    return n;
}

std::vector<Notification> NotificationRepository::findByUserId(int userId, bool unreadOnly) {
    std::vector<Notification> list;
    QString sql = "SELECT * FROM notifications WHERE (user_id = ? OR user_id IS NULL) ";
    if (unreadOnly) {
        sql += "AND is_read = 0 ";
    }
    sql += "ORDER BY notification_id DESC;";

    QSqlQuery query = DatabaseManager::instance().prepare(sql);
    query.addBindValue(userId);

    if (query.exec()) {
        while (query.next()) {
            list.push_back(mapNotification(query));
        }
    }
    return list;
}

int NotificationRepository::getUnreadCount(int userId) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT COUNT(*) FROM notifications WHERE (user_id = ? OR user_id IS NULL) AND is_read = 0;"
    );
    query.addBindValue(userId);
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

bool NotificationRepository::create(Notification& notification) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "INSERT INTO notifications (user_id, title, message, type, is_read) VALUES (?, ?, ?, ?, ?);"
    );
    query.addBindValue(notification.getUserId() > 0 ? QVariant(notification.getUserId()) : QVariant(QVariant::Int));
    query.addBindValue(notification.getTitle());
    query.addBindValue(notification.getMessage());
    query.addBindValue(notification.getType());
    query.addBindValue(notification.isRead() ? 1 : 0);

    if (query.exec()) {
        notification.setId(query.lastInsertId().toInt());
        return true;
    }
    return false;
}

bool NotificationRepository::markAsRead(int notificationId) {
    QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QSqlQuery query = DatabaseManager::instance().prepare(
        "UPDATE notifications SET is_read = 1, read_at = ? WHERE notification_id = ?;"
    );
    query.addBindValue(now);
    query.addBindValue(notificationId);
    return query.exec();
}

bool NotificationRepository::markAllAsRead(int userId) {
    QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QSqlQuery query = DatabaseManager::instance().prepare(
        "UPDATE notifications SET is_read = 1, read_at = ? WHERE (user_id = ? OR user_id IS NULL) AND is_read = 0;"
    );
    query.addBindValue(now);
    query.addBindValue(userId);
    return query.exec();
}

} // namespace FitCore
