#ifndef NOTIFICATIONSERVICE_H
#define NOTIFICATIONSERVICE_H

#include "models/Notification.h"
#include "repositories/NotificationRepository.h"

namespace FitCore {

class NotificationService {
public:
    NotificationService() = default;

    void checkAndGenerateExpiriesAndAlerts();
    std::vector<Notification> getUserNotifications(int userId, bool unreadOnly = false);
    int getUnreadCount(int userId);
    bool markAsRead(int notificationId);
    bool markAllAsRead(int userId);
};

} // namespace FitCore

#endif // NOTIFICATIONSERVICE_H
