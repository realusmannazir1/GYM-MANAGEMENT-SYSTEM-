#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include "models/User.h"
#include <optional>
#include <vector>
#include <QString>

namespace FitCore {

class UserRepository {
public:
    UserRepository() = default;

    std::optional<User> findById(int id);
    std::optional<User> findByUsername(const QString& username);
    std::vector<User> findAll();
    bool create(User& user);
    bool update(const User& user);
    bool remove(int id);
    bool updatePassword(int userId, const QString& newHash, const QString& newSalt);
};

} // namespace FitCore

#endif // USERREPOSITORY_H
