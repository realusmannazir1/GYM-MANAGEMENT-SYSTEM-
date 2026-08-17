#ifndef MEMBERSHIPSERVICE_H
#define MEMBERSHIPSERVICE_H

#include "models/Membership.h"
#include "models/MembershipPlan.h"
#include "repositories/MembershipRepository.h"
#include "repositories/MembershipPlanRepository.h"
#include "repositories/MemberRepository.h"
#include "repositories/PaymentRepository.h"
#include "repositories/AuditRepository.h"
#include <QString>

namespace FitCore {

struct ServiceResult {
    bool success{false};
    QString message;
    int entityId{0};
};

class MembershipService {
public:
    MembershipService() = default;

    ServiceResult renewMembership(int memberId, int planId, double discountAmount, const QString& paymentMethod, int currentUserId);
    ServiceResult cancelMembership(int membershipId, int currentUserId);
    void updateAutoExpiries();
};

} // namespace FitCore

#endif // MEMBERSHIPSERVICE_H
