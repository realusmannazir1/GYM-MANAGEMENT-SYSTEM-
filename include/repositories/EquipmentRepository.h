#ifndef EQUIPMENTREPOSITORY_H
#define EQUIPMENTREPOSITORY_H

#include "models/Equipment.h"
#include <optional>
#include <vector>

namespace FitCore {

struct MaintenanceLogItem {
    int maintenanceId{0};
    int equipmentId{0};
    QString equipmentName;
    QString equipmentCode;
    QString maintenanceDate;
    QString problemDescription;
    QString serviceDescription;
    double cost{0.0};
    QString technicianName;
    QString nextMaintenanceDate;
    QString status;
};

class EquipmentRepository {
public:
    EquipmentRepository() = default;

    std::optional<Equipment> findById(int id);
    std::optional<Equipment> findByCode(const QString& code);
    std::vector<Equipment> findAll(const QString& categoryFilter = "");
    bool create(Equipment& equipment);
    bool update(const Equipment& equipment);
    bool remove(int id);
    QString generateNextEquipmentCode();

    // Maintenance
    bool addMaintenanceRecord(MaintenanceLogItem& item);
    std::vector<MaintenanceLogItem> getMaintenanceHistory(int equipmentId);
    std::vector<MaintenanceLogItem> getUpcomingMaintenanceAlerts(int withinDays = 14);
};

} // namespace FitCore

#endif // EQUIPMENTREPOSITORY_H
