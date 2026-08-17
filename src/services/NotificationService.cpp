#include "services/NotificationService.h"
#include "repositories/MembershipRepository.h"
#include "repositories/EquipmentRepository.h"
#include <QDebug>

namespace FitCore {

void NotificationService::checkAndGenerateExpiriesAndAlerts() {
    MembershipRepository msRepo;
    EquipmentRepository eqRepo;
    NotificationRepository notifRepo;

    // Check memberships expiring within 7 days
    auto expiring = msRepo.findExpiringSoon(7);
    for (const auto& ms : expiring) {
        Notification n;
        n.setTitle("Membership Expiring Soon");
        n.setMessage(QString("Member %1 (%2) membership expires on %3.")
                         .arg(ms.getMemberName(), ms.getPlanName(), ms.getEndDate()));
        n.setType("Expiry");
        notifRepo.create(n);
    }

    // Check equipment maintenance alerts
    auto maintenance = eqRepo.getUpcomingMaintenanceAlerts(14);
    for (const auto& m : maintenance) {
        Notification n;
        n.setTitle("Equipment Maintenance Alert");
        n.setMessage(QString("Equipment %1 (%2) maintenance scheduled for %3.")
                         .arg(m.equipmentName, m.equipmentCode, m.nextMaintenanceDate));
        n.setType("Alert");
        notifRepo.create(n);
    }
}

std::vector<Notification> NotificationService::getUserNotifications(int userId, bool unreadOnly) {
    NotificationRepository repo;
    return repo.findByUserId(userId, unreadOnly);
}

int NotificationService::getUnreadCount(int userId) {
    NotificationRepository repo;
    return repo.getUnreadCount(userId);
}

bool NotificationService::markAsRead(int notificationId) {
    NotificationRepository repo;
    return repo.markAsRead(notificationId);
}

bool NotificationService::markAllAsRead(int userId) {
    NotificationRepository repo;
    return repo.markAllAsRead(userId);
}

} // namespace FitCore
