#ifndef AUTHENTICATIONS SERVICE_H
#define AUTHENTICATIONSERVICE_H

#include "models/User.h"
#include "repositories/UserRepository.h"
#include "repositories/AuditRepository.h"
#include <optional>
#include <QString>

namespace FitCore {

struct AuthResult {
    bool success{false};
    QString message;
    std::optional<User> user;
};

class AuthenticationService {
public:
    static AuthenticationService& instance();

    AuthResult login(const QString& username, const QString& password);
    void logout();

    bool isLoggedIn() const;
    std::optional<User> currentUser() const;
    UserRole currentRole() const;

    bool hasPermission(UserRole requiredRole) const;

    static QString hashPassword(const QString& password, const QString& salt);
    static QString generateSalt();

private:
    AuthenticationService() = default;

    UserRepository m_userRepo;
    AuditRepository m_auditRepo;
    std::optional<User> m_currentUser;
};

} // namespace FitCore

#endif // AUTHENTICATIONSERVICE_H
