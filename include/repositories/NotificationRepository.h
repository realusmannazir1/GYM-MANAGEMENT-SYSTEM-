#ifndef NOTIFICATIONREPOSITORY_H
#define NOTIFICATIONREPOSITORY_H

#include "models/Notification.h"
#include <vector>

namespace FitCore {

class NotificationRepository {
public:
    NotificationRepository() = default;

    std::vector<Notification> findByUserId(int userId, bool unreadOnly = false);
    int getUnreadCount(int userId);
    bool create(Notification& notification);
    bool markAsRead(int notificationId);
    bool markAllAsRead(int userId);
};

} // namespace FitCore

#endif // NOTIFICATIONREPOSITORY_H
