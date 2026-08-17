#ifndef MEMBERSHIPREPOSITORY_H
#define MEMBERSHIPREPOSITORY_H

#include "models/Membership.h"
#include <optional>
#include <vector>

namespace FitCore {

class MembershipRepository {
public:
    MembershipRepository() = default;

    std::optional<Membership> findById(int id);
    std::optional<Membership> findActiveByMemberId(int memberId);
    std::vector<Membership> findHistoryByMemberId(int memberId);
    std::vector<Membership> findAll(const QString& statusFilter = "");
    std::vector<Membership> findExpiringSoon(int withinDays = 7);
    std::vector<Membership> findExpired();

    bool create(Membership& membership);
    bool update(const Membership& membership);
    bool setStatus(int membershipId, const QString& status);
    bool autoUpdateExpiredStatuses();
};

} // namespace FitCore

#endif // MEMBERSHIPREPOSITORY_H
