#ifndef MEMBERSHIPPLANREPOSITORY_H
#define MEMBERSHIPPLANREPOSITORY_H

#include "models/MembershipPlan.h"
#include <optional>
#include <vector>

namespace FitCore {

class MembershipPlanRepository {
public:
    MembershipPlanRepository() = default;

    std::optional<MembershipPlan> findById(int id);
    std::vector<MembershipPlan> findAll(bool activeOnly = true);
    bool create(MembershipPlan& plan);
    bool update(const MembershipPlan& plan);
    bool setStatus(int id, const QString& status);
};

} // namespace FitCore

#endif // MEMBERSHIPPLANREPOSITORY_H
