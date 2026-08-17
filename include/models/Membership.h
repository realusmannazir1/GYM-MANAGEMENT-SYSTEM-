#ifndef MEMBERSHIP_H
#define MEMBERSHIP_H

#include <QString>
#include <QDate>

namespace FitCore {

class Membership {
public:
    Membership();

    int getId() const;
    void setId(int id);

    int getMemberId() const;
    void setMemberId(int memberId);

    int getPlanId() const;
    void setPlanId(int planId);

    QString getPlanName() const;
    void setPlanName(const QString& name);

    QString getMemberName() const;
    void setMemberName(const QString& name);

    QString getStartDate() const;
    void setStartDate(const QString& startDate);

    QString getEndDate() const;
    void setEndDate(const QString& endDate);

    double getPrice() const;
    void setPrice(double price);

    double getDiscount() const;
    void setDiscount(double discount);

    double getFinalAmount() const;
    void setFinalAmount(double amount);

    QString getStatus() const;
    void setStatus(const QString& status);

    QString getPaymentStatus() const;
    void setPaymentStatus(const QString& status);

    int getCreatedBy() const;
    void setCreatedBy(int userId);

    bool isExpired() const;
    bool isExpiringSoon(int days = 7) const;

private:
    int m_id;
    int m_memberId;
    int m_planId;
    QString m_planName;
    QString m_memberName;
    QString m_startDate;
    QString m_endDate;
    double m_price;
    double m_discount;
    double m_finalAmount;
    QString m_status;
    QString m_paymentStatus;
    int m_createdBy;
};

} // namespace FitCore

#endif // MEMBERSHIP_H
