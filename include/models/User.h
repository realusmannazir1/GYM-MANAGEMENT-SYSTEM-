#ifndef USER_H
#define USER_H

#include <QString>
#include <QDateTime>

namespace FitCore {

enum class UserRole {
    Admin = 1,
    Manager = 2,
    Receptionist = 3,
    Trainer = 4,
    Unknown = 0
};

class User {
public:
    User();
    User(int id, const QString& username, const QString& fullName, const QString& email,
         const QString& phone, UserRole role, bool isActive = true);

    int getId() const;
    void setId(int id);

    QString getUsername() const;
    void setUsername(const QString& username);

    QString getPasswordHash() const;
    void setPasswordHash(const QString& hash);

    QString getSalt() const;
    void setSalt(const QString& salt);

    QString getFullName() const;
    void setFullName(const QString& fullName);

    QString getEmail() const;
    void setEmail(const QString& email);

    QString getPhone() const;
    void setPhone(const QString& phone);

    UserRole getRole() const;
    void setRole(UserRole role);
    QString getRoleName() const;

    bool isActive() const;
    void setActive(bool active);

    QString getCreatedAt() const;
    void setCreatedAt(const QString& createdAt);

    // Role check helpers
    bool isAdmin() const;
    bool isManager() const;
    bool isReceptionist() const;
    bool isTrainer() const;

    static UserRole roleFromInt(int roleId);
    static int roleToInt(UserRole role);
    static QString roleToString(UserRole role);

private:
    int m_id;
    QString m_username;
    QString m_passwordHash;
    QString m_salt;
    QString m_fullName;
    QString m_email;
    QString m_phone;
    UserRole m_role;
    bool m_isActive;
    QString m_createdAt;
};

} // namespace FitCore

#endif // USER_H
