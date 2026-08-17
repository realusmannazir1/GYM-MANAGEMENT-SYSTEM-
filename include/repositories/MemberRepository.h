#ifndef MEMBERREPOSITORY_H
#define MEMBERREPOSITORY_H

#include "models/Member.h"
#include <optional>
#include <vector>
#include <QString>

namespace FitCore {

class MemberRepository {
public:
    MemberRepository() = default;

    std::optional<Member> findById(int id);
    std::optional<Member> findByMembershipNumber(const QString& number);
    std::vector<Member> findAll();
    std::vector<Member> findAllPaged(int limit, int offset, const QString& statusFilter = "");
    std::vector<Member> search(const QString& keyword);
    int count(const QString& statusFilter = "");

    bool create(Member& member);
    bool update(const Member& member);
    bool remove(int id);
    bool setStatus(int id, const QString& status);

    QString generateNextMembershipNumber();
};

} // namespace FitCore

#endif // MEMBERREPOSITORY_H
