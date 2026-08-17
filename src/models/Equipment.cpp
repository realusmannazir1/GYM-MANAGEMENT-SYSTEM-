#include "models/Equipment.h"

namespace FitCore {

Equipment::Equipment()
    : m_id(0), m_purchasePrice(0.0), m_condition("Good"),
      m_status("Available"), m_location("Main Gym Floor") {}

int Equipment::getId() const { return m_id; }
void Equipment::setId(int id) { m_id = id; }

QString Equipment::getEquipmentCode() const { return m_equipmentCode; }
void Equipment::setEquipmentCode(const QString& code) { m_equipmentCode = code; }

QString Equipment::getEquipmentName() const { return m_equipmentName; }
void Equipment::setEquipmentName(const QString& name) { m_equipmentName = name; }

QString Equipment::getCategory() const { return m_category; }
void Equipment::setCategory(const QString& category) { m_category = category; }

QString Equipment::getBrand() const { return m_brand; }
void Equipment::setBrand(const QString& brand) { m_brand = brand; }

QString Equipment::getModel() const { return m_model; }
void Equipment::setModel(const QString& model) { m_model = model; }

QString Equipment::getPurchaseDate() const { return m_purchaseDate; }
void Equipment::setPurchaseDate(const QString& date) { m_purchaseDate = date; }

double Equipment::getPurchasePrice() const { return m_purchasePrice; }
void Equipment::setPurchasePrice(double price) { m_purchasePrice = price; }

QString Equipment::getWarrantyExpiry() const { return m_warrantyExpiry; }
void Equipment::setWarrantyExpiry(const QString& date) { m_warrantyExpiry = date; }

QString Equipment::getCondition() const { return m_condition; }
void Equipment::setCondition(const QString& condition) { m_condition = condition; }

QString Equipment::getStatus() const { return m_status; }
void Equipment::setStatus(const QString& status) { m_status = status; }

QString Equipment::getLocation() const { return m_location; }
void Equipment::setLocation(const QString& location) { m_location = location; }

QString Equipment::getNotes() const { return m_notes; }
void Equipment::setNotes(const QString& notes) { m_notes = notes; }

} // namespace FitCore
