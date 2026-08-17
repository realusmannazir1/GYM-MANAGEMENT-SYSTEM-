#include "models/User.h"

namespace FitCore {

User::User()
    : m_id(0), m_role(UserRole::Unknown), m_isActive(true) {}

User::User(int id, const QString& username, const QString& fullName, const QString& email,
           const QString& phone, UserRole role, bool isActive)
    : m_id(id), m_username(username), m_fullName(fullName), m_email(email),
      m_phone(phone), m_role(role), m_isActive(isActive) {}

int User::getId() const { return m_id; }
void User::setId(int id) { m_id = id; }

QString User::getUsername() const { return m_username; }
void User::setUsername(const QString& username) { m_username = username; }

QString User::getPasswordHash() const { return m_passwordHash; }
void User::setPasswordHash(const QString& hash) { m_passwordHash = hash; }

QString User::getSalt() const { return m_salt; }
void User::setSalt(const QString& salt) { m_salt = salt; }

QString User::getFullName() const { return m_fullName; }
void User::setFullName(const QString& fullName) { m_fullName = fullName; }

QString User::getEmail() const { return m_email; }
void User::setEmail(const QString& email) { m_email = email; }

QString User::getPhone() const { return m_phone; }
void User::setPhone(const QString& phone) { m_phone = phone; }

UserRole User::getRole() const { return m_role; }
void User::setRole(UserRole role) { m_role = role; }

QString User::getRoleName() const { return roleToString(m_role); }

bool User::isActive() const { return m_isActive; }
void User::setActive(bool active) { m_isActive = active; }

QString User::getCreatedAt() const { return m_createdAt; }
void User::setCreatedAt(const QString& createdAt) { m_createdAt = createdAt; }

bool User::isAdmin() const { return m_role == UserRole::Admin; }
bool User::isManager() const { return m_role == UserRole::Manager; }
bool User::isReceptionist() const { return m_role == UserRole::Receptionist; }
bool User::isTrainer() const { return m_role == UserRole::Trainer; }

UserRole User::roleFromInt(int roleId) {
    switch (roleId) {
        case 1: return UserRole::Admin;
        case 2: return UserRole::Manager;
        case 3: return UserRole::Receptionist;
        case 4: return UserRole::Trainer;
        default: return UserRole::Unknown;
    }
}

int User::roleToInt(UserRole role) {
    return static_cast<int>(role);
}

QString User::roleToString(UserRole role) {
    switch (role) {
        case UserRole::Admin: return "Admin";
        case UserRole::Manager: return "Manager";
        case UserRole::Receptionist: return "Receptionist";
        case UserRole::Trainer: return "Trainer";
        default: return "Unknown";
    }
}

} // namespace FitCore
// Incremental compilation verification comment

