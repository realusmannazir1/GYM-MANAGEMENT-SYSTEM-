#ifndef MEMBERSHIPPLAN_H
#define MEMBERSHIPPLAN_H

#include <QString>

namespace FitCore {

class MembershipPlan {
public:
    MembershipPlan();
    MembershipPlan(int id, const QString& name, int durationDays, double price, const QString& description);

    int getId() const;
    void setId(int id);

    QString getPlanName() const;
    void setPlanName(const QString& name);

    int getDurationDays() const;
    void setDurationDays(int days);

    double getPrice() const;
    void setPrice(double price);

    QString getDescription() const;
    void setDescription(const QString& description);

    QString getAccessType() const;
    void setAccessType(const QString& accessType);

    QString getStatus() const;
    void setStatus(const QString& status);

private:
    int m_id;
    QString m_planName;
    int m_durationDays;
    double m_price;
    QString m_description;
    QString m_accessType;
    QString m_status;
};

} // namespace FitCore

#endif // MEMBERSHIPPLAN_H
