#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <QString>

namespace FitCore {

class Equipment {
public:
    Equipment();

    int getId() const;
    void setId(int id);

    QString getEquipmentCode() const;
    void setEquipmentCode(const QString& code);

    QString getEquipmentName() const;
    void setEquipmentName(const QString& name);

    QString getCategory() const;
    void setCategory(const QString& category);

    QString getBrand() const;
    void setBrand(const QString& brand);

    QString getModel() const;
    void setModel(const QString& model);

    QString getPurchaseDate() const;
    void setPurchaseDate(const QString& date);

    double getPurchasePrice() const;
    void setPurchasePrice(double price);

    QString getWarrantyExpiry() const;
    void setWarrantyExpiry(const QString& date);

    QString getCondition() const;
    void setCondition(const QString& condition);

    QString getStatus() const;
    void setStatus(const QString& status);

    QString getLocation() const;
    void setLocation(const QString& location);

    QString getNotes() const;
    void setNotes(const QString& notes);

private:
    int m_id;
    QString m_equipmentCode;
    QString m_equipmentName;
    QString m_category;
    QString m_brand;
    QString m_model;
    QString m_purchaseDate;
    double m_purchasePrice;
    QString m_warrantyExpiry;
    QString m_condition;
    QString m_status;
    QString m_location;
    QString m_notes;
};

} // namespace FitCore

#endif // EQUIPMENT_H
