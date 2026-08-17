#ifndef PROGRESSREPOSITORY_H
#define PROGRESSREPOSITORY_H

#include "models/ProgressRecord.h"
#include <optional>
#include <vector>

namespace FitCore {

class ProgressRepository {
public:
    ProgressRepository() = default;

    std::optional<ProgressRecord> findById(int id);
    std::vector<ProgressRecord> findByMemberId(int memberId);
    std::optional<ProgressRecord> findLatestByMemberId(int memberId);
    bool create(ProgressRecord& record);
    bool update(const ProgressRecord& record);
    bool remove(int id);
};

} // namespace FitCore

#endif // PROGRESSREPOSITORY_H
