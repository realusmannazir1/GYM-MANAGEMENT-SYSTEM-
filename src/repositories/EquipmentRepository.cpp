#include "repositories/EquipmentRepository.h"
#include "database/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDate>

namespace FitCore {

static Equipment mapEquipment(const QSqlQuery& query) {
    Equipment e;
    e.setId(query.value("equipment_id").toInt());
    e.setEquipmentCode(query.value("equipment_code").toString());
    e.setEquipmentName(query.value("equipment_name").toString());
    e.setCategory(query.value("category").toString());
    e.setBrand(query.value("brand").toString());
    e.setModel(query.value("model").toString());
    e.setPurchaseDate(query.value("purchase_date").toString());
    e.setPurchasePrice(query.value("purchase_price").toDouble());
    e.setWarrantyExpiry(query.value("warranty_expiry").toString());
    e.setCondition(query.value("condition").toString());
    e.setStatus(query.value("status").toString());
    e.setLocation(query.value("location").toString());
    e.setNotes(query.value("notes").toString());
    return e;
}

std::optional<Equipment> EquipmentRepository::findById(int id) {
    QSqlQuery query = DatabaseManager::instance().prepare("SELECT * FROM equipment WHERE equipment_id = ?;");
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        return mapEquipment(query);
    }
    return std::nullopt;
}

std::optional<Equipment> EquipmentRepository::findByCode(const QString& code) {
    QSqlQuery query = DatabaseManager::instance().prepare("SELECT * FROM equipment WHERE UPPER(equipment_code) = UPPER(?);");
    query.addBindValue(code.trimmed());
    if (query.exec() && query.next()) {
        return mapEquipment(query);
    }
    return std::nullopt;
}

std::vector<Equipment> EquipmentRepository::findAll(const QString& categoryFilter) {
    std::vector<Equipment> list;
    QString sql = "SELECT * FROM equipment ";
    if (!categoryFilter.isEmpty() && categoryFilter != "All") {
        sql += "WHERE category = '" + categoryFilter + "' ";
    }
    sql += "ORDER BY equipment_id DESC;";

    QSqlQuery query(sql, DatabaseManager::instance().getDatabase());
    while (query.next()) {
        list.push_back(mapEquipment(query));
    }
    return list;
}

bool EquipmentRepository::create(Equipment& equipment) {
    if (equipment.getEquipmentCode().isEmpty()) {
        equipment.setEquipmentCode(generateNextEquipmentCode());
    }

    QSqlQuery query = DatabaseManager::instance().prepare(
        "INSERT INTO equipment (equipment_code, equipment_name, category, brand, model, purchase_date, purchase_price, warranty_expiry, condition, status, location, notes) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"
    );
    query.addBindValue(equipment.getEquipmentCode());
    query.addBindValue(equipment.getEquipmentName());
    query.addBindValue(equipment.getCategory());
    query.addBindValue(equipment.getBrand());
    query.addBindValue(equipment.getModel());
    query.addBindValue(equipment.getPurchaseDate());
    query.addBindValue(equipment.getPurchasePrice());
    query.addBindValue(equipment.getWarrantyExpiry());
    query.addBindValue(equipment.getCondition());
    query.addBindValue(equipment.getStatus());
    query.addBindValue(equipment.getLocation());
    query.addBindValue(equipment.getNotes());

    if (query.exec()) {
        equipment.setId(query.lastInsertId().toInt());
        return true;
    }
    return false;
}

bool EquipmentRepository::update(const Equipment& equipment) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "UPDATE equipment SET equipment_name = ?, category = ?, brand = ?, model = ?, "
        "purchase_date = ?, purchase_price = ?, warranty_expiry = ?, condition = ?, "
        "status = ?, location = ?, notes = ? WHERE equipment_id = ?;"
    );
    query.addBindValue(equipment.getEquipmentName());
    query.addBindValue(equipment.getCategory());
    query.addBindValue(equipment.getBrand());
    query.addBindValue(equipment.getModel());
    query.addBindValue(equipment.getPurchaseDate());
    query.addBindValue(equipment.getPurchasePrice());
    query.addBindValue(equipment.getWarrantyExpiry());
    query.addBindValue(equipment.getCondition());
    query.addBindValue(equipment.getStatus());
    query.addBindValue(equipment.getLocation());
    query.addBindValue(equipment.getNotes());
    query.addBindValue(equipment.getId());
    return query.exec();
}

bool EquipmentRepository::remove(int id) {
    QSqlQuery query = DatabaseManager::instance().prepare("UPDATE equipment SET status = 'Retired' WHERE equipment_id = ?;");
    query.addBindValue(id);
    return query.exec();
}

QString EquipmentRepository::generateNextEquipmentCode() {
    QSqlQuery query("SELECT MAX(equipment_id) FROM equipment;", DatabaseManager::instance().getDatabase());
    int maxId = 0;
    if (query.next()) maxId = query.value(0).toInt();
    return QString("EQP-%1").arg(maxId + 1, 6, 10, QChar('0'));
}

bool EquipmentRepository::addMaintenanceRecord(MaintenanceLogItem& item) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "INSERT INTO equipment_maintenance (equipment_id, maintenance_date, problem_description, service_description, cost, technician_name, next_maintenance_date, status) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?);"
    );
    query.addBindValue(item.equipmentId);
    query.addBindValue(item.maintenanceDate.isEmpty() ? QDate::currentDate().toString("yyyy-MM-dd") : item.maintenanceDate);
    query.addBindValue(item.problemDescription);
    query.addBindValue(item.serviceDescription);
    query.addBindValue(item.cost);
    query.addBindValue(item.technicianName);
    query.addBindValue(item.nextMaintenanceDate);
    query.addBindValue(item.status);

    if (query.exec()) {
        item.maintenanceId = query.lastInsertId().toInt();
        // Update equipment status
        if (item.status == "In Progress" || item.status == "Pending") {
            QSqlQuery updateEq("UPDATE equipment SET status = 'Maintenance' WHERE equipment_id = " + QString::number(item.equipmentId) + ";", DatabaseManager::instance().getDatabase());
            updateEq.exec();
        } else if (item.status == "Completed") {
            QSqlQuery updateEq("UPDATE equipment SET status = 'Available' WHERE equipment_id = " + QString::number(item.equipmentId) + ";", DatabaseManager::instance().getDatabase());
            updateEq.exec();
        }
        return true;
    }
    return false;
}

std::vector<MaintenanceLogItem> EquipmentRepository::getMaintenanceHistory(int equipmentId) {
    std::vector<MaintenanceLogItem> list;
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT em.*, e.equipment_name, e.equipment_code "
        "FROM equipment_maintenance em "
        "JOIN equipment e ON em.equipment_id = e.equipment_id "
        "WHERE em.equipment_id = ? ORDER BY em.maintenance_id DESC;"
    );
    query.addBindValue(equipmentId);
    if (query.exec()) {
        while (query.next()) {
            MaintenanceLogItem item;
            item.maintenanceId = query.value("maintenance_id").toInt();
            item.equipmentId = query.value("equipment_id").toInt();
            item.equipmentName = query.value("equipment_name").toString();
            item.equipmentCode = query.value("equipment_code").toString();
            item.maintenanceDate = query.value("maintenance_date").toString();
            item.problemDescription = query.value("problem_description").toString();
            item.serviceDescription = query.value("service_description").toString();
            item.cost = query.value("cost").toDouble();
            item.technicianName = query.value("technician_name").toString();
            item.nextMaintenanceDate = query.value("next_maintenance_date").toString();
            item.status = query.value("status").toString();
            list.push_back(item);
        }
    }
    return list;
}

std::vector<MaintenanceLogItem> EquipmentRepository::getUpcomingMaintenanceAlerts(int withinDays) {
    std::vector<MaintenanceLogItem> list;
    QString today = QDate::currentDate().toString("yyyy-MM-dd");
    QString targetDate = QDate::currentDate().addDays(withinDays).toString("yyyy-MM-dd");

    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT em.*, e.equipment_name, e.equipment_code "
        "FROM equipment_maintenance em "
        "JOIN equipment e ON em.equipment_id = e.equipment_id "
        "WHERE (em.status = 'Pending' OR em.status = 'In Progress' OR (em.next_maintenance_date >= ? AND em.next_maintenance_date <= ?)) "
        "ORDER BY em.next_maintenance_date ASC;"
    );
    query.addBindValue(today);
    query.addBindValue(targetDate);

    if (query.exec()) {
        while (query.next()) {
            MaintenanceLogItem item;
            item.maintenanceId = query.value("maintenance_id").toInt();
            item.equipmentId = query.value("equipment_id").toInt();
            item.equipmentName = query.value("equipment_name").toString();
            item.equipmentCode = query.value("equipment_code").toString();
            item.maintenanceDate = query.value("maintenance_date").toString();
            item.problemDescription = query.value("problem_description").toString();
            item.serviceDescription = query.value("service_description").toString();
            item.cost = query.value("cost").toDouble();
            item.technicianName = query.value("technician_name").toString();
            item.nextMaintenanceDate = query.value("next_maintenance_date").toString();
            item.status = query.value("status").toString();
            list.push_back(item);
        }
    }
    return list;
}

} // namespace FitCore
