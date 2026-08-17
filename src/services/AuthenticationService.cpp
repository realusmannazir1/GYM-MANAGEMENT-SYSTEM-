#include "services/AuthenticationService.h"
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QDateTime>
#include <QDebug>

namespace FitCore {

AuthenticationService& AuthenticationService::instance() {
    static AuthenticationService instance;
    return instance;
}

AuthResult AuthenticationService::login(const QString& username, const QString& password) {
    AuthResult res;
    if (username.trimmed().isEmpty() || password.isEmpty()) {
        res.message = "Username and password cannot be empty.";
        return res;
    }

    auto userOpt = m_userRepo.findByUsername(username);
    if (!userOpt.has_value()) {
        res.message = "Invalid username or password.";
        return res;
    }

    User user = userOpt.value();
    if (!user.isActive()) {
        res.message = "Account is disabled. Please contact system administrator.";
        return res;
    }

    QString computedHash = hashPassword(password, user.getSalt());
    if (computedHash != user.getPasswordHash()) {
        res.message = "Invalid username or password.";
        return res;
    }

    m_currentUser = user;
    res.success = true;
    res.user = user;
    res.message = "Login successful!";

    m_auditRepo.logAction(user.getId(), "LOGIN", "User", user.getId(), "User " + user.getUsername() + " logged in successfully.");
    return res;
}

void AuthenticationService::logout() {
    if (m_currentUser.has_value()) {
        m_auditRepo.logAction(m_currentUser->getId(), "LOGOUT", "User", m_currentUser->getId(), "User " + m_currentUser->getUsername() + " logged out.");
        m_currentUser.reset();
    }
}

bool AuthenticationService::isLoggedIn() const {
    return m_currentUser.has_value();
}

std::optional<User> AuthenticationService::currentUser() const {
    return m_currentUser;
}

UserRole AuthenticationService::currentRole() const {
    if (m_currentUser.has_value()) {
        return m_currentUser->getRole();
    }
    return UserRole::Unknown;
}

bool AuthenticationService::hasPermission(UserRole requiredRole) const {
    if (!m_currentUser.has_value()) return false;
    UserRole current = m_currentUser->getRole();
    if (current == UserRole::Admin) return true; // Admin has access to everything
    return current == requiredRole;
}

QString AuthenticationService::hashPassword(const QString& password, const QString& salt) {
    QByteArray combined = (password + salt).toUtf8();
    QByteArray hash = QCryptographicHash::hash(combined, QCryptographicHash::Sha256);
    return QString(hash.toHex());
}

QString AuthenticationService::generateSalt() {
    const QString chars("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    QString salt;
    for (int i = 0; i < 16; ++i) {
        int index = QRandomGenerator::global()->bounded(chars.length());
        salt.append(chars.at(index));
    }
    return salt;
}

} // namespace FitCore
