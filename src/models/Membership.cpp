#include "models/Membership.h"

namespace FitCore {

Membership::Membership()
    : m_id(0), m_memberId(0), m_planId(0), m_price(0.0), m_discount(0.0),
      m_finalAmount(0.0), m_status("Active"), m_paymentStatus("Paid"), m_createdBy(1) {}

int Membership::getId() const { return m_id; }
void Membership::setId(int id) { m_id = id; }

int Membership::getMemberId() const { return m_memberId; }
void Membership::setMemberId(int memberId) { m_memberId = memberId; }

int Membership::getPlanId() const { return m_planId; }
void Membership::setPlanId(int planId) { m_planId = planId; }

QString Membership::getPlanName() const { return m_planName; }
void Membership::setPlanName(const QString& name) { m_planName = name; }

QString Membership::getMemberName() const { return m_memberName; }
void Membership::setMemberName(const QString& name) { m_memberName = name; }

QString Membership::getStartDate() const { return m_startDate; }
void Membership::setStartDate(const QString& startDate) { m_startDate = startDate; }

QString Membership::getEndDate() const { return m_endDate; }
void Membership::setEndDate(const QString& endDate) { m_endDate = endDate; }

double Membership::getPrice() const { return m_price; }
void Membership::setPrice(double price) { m_price = price; }

double Membership::getDiscount() const { return m_discount; }
void Membership::setDiscount(double discount) { m_discount = discount; }

double Membership::getFinalAmount() const { return m_finalAmount; }
void Membership::setFinalAmount(double amount) { m_finalAmount = amount; }

QString Membership::getStatus() const { return m_status; }
void Membership::setStatus(const QString& status) { m_status = status; }

QString Membership::getPaymentStatus() const { return m_paymentStatus; }
void Membership::setPaymentStatus(const QString& status) { m_paymentStatus = status; }

int Membership::getCreatedBy() const { return m_createdBy; }
void Membership::setCreatedBy(int userId) { m_createdBy = userId; }

bool Membership::isExpired() const {
    if (m_endDate.isEmpty()) return false;
    QDate endDate = QDate::fromString(m_endDate, "yyyy-MM-dd");
    return endDate.isValid() && endDate < QDate::currentDate();
}

bool Membership::isExpiringSoon(int days) const {
    if (m_endDate.isEmpty()) return false;
    QDate endDate = QDate::fromString(m_endDate, "yyyy-MM-dd");
    QDate today = QDate::currentDate();
    return endDate.isValid() && endDate >= today && endDate <= today.addDays(days);
}

} // namespace FitCore
