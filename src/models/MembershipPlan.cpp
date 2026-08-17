#include "models/MembershipPlan.h"

namespace FitCore {

MembershipPlan::MembershipPlan()
    : m_id(0), m_durationDays(30), m_price(0.0), m_accessType("All-Access"), m_status("Active") {}

MembershipPlan::MembershipPlan(int id, const QString& name, int durationDays, double price, const QString& description)
    : m_id(id), m_planName(name), m_durationDays(durationDays), m_price(price), m_description(description), m_accessType("All-Access"), m_status("Active") {}

int MembershipPlan::getId() const { return m_id; }
void MembershipPlan::setId(int id) { m_id = id; }

QString MembershipPlan::getPlanName() const { return m_planName; }
void MembershipPlan::setPlanName(const QString& name) { m_planName = name; }

int MembershipPlan::getDurationDays() const { return m_durationDays; }
void MembershipPlan::setDurationDays(int days) { m_durationDays = days; }

double MembershipPlan::getPrice() const { return m_price; }
void MembershipPlan::setPrice(double price) { m_price = price; }

QString MembershipPlan::getDescription() const { return m_description; }
void MembershipPlan::setDescription(const QString& description) { m_description = description; }

QString MembershipPlan::getAccessType() const { return m_accessType; }
void MembershipPlan::setAccessType(const QString& accessType) { m_accessType = accessType; }

QString MembershipPlan::getStatus() const { return m_status; }
void MembershipPlan::setStatus(const QString& status) { m_status = status; }

} // namespace FitCore
