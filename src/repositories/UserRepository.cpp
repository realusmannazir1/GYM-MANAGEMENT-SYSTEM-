#include "repositories/UserRepository.h"
#include "database/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

namespace FitCore {

std::optional<User> UserRepository::findById(int id) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT user_id, username, password_hash, salt, full_name, email, phone, role_id, is_active, created_at "
        "FROM users WHERE user_id = ?;"
    );
    query.addBindValue(id);

    if (query.exec() && query.next()) {
        User u(
            query.value(0).toInt(),
            query.value(1).toString(),
            query.value(4).toString(),
            query.value(5).toString(),
            query.value(6).toString(),
            User::roleFromInt(query.value(7).toInt()),
            query.value(8).toInt() == 1
        );
        u.setPasswordHash(query.value(2).toString());
        u.setSalt(query.value(3).toString());
        u.setCreatedAt(query.value(9).toString());
        return u;
    }
    return std::nullopt;
}

std::optional<User> UserRepository::findByUsername(const QString& username) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT user_id, username, password_hash, salt, full_name, email, phone, role_id, is_active, created_at "
        "FROM users WHERE LOWER(username) = LOWER(?);"
    );
    query.addBindValue(username.trimmed());

    if (query.exec() && query.next()) {
        User u(
            query.value(0).toInt(),
            query.value(1).toString(),
            query.value(4).toString(),
            query.value(5).toString(),
            query.value(6).toString(),
            User::roleFromInt(query.value(7).toInt()),
            query.value(8).toInt() == 1
        );
        u.setPasswordHash(query.value(2).toString());
        u.setSalt(query.value(3).toString());
        u.setCreatedAt(query.value(9).toString());
        return u;
    }
    return std::nullopt;
}

std::vector<User> UserRepository::findAll() {
    std::vector<User> list;
    QSqlQuery query(DatabaseManager::instance().getDatabase());
    if (query.exec("SELECT user_id, username, password_hash, salt, full_name, email, phone, role_id, is_active, created_at FROM users ORDER BY user_id ASC;")) {
        while (query.next()) {
            User u(
                query.value(0).toInt(),
                query.value(1).toString(),
                query.value(4).toString(),
                query.value(5).toString(),
                query.value(6).toString(),
                User::roleFromInt(query.value(7).toInt()),
                query.value(8).toInt() == 1
            );
            u.setPasswordHash(query.value(2).toString());
            u.setSalt(query.value(3).toString());
            u.setCreatedAt(query.value(9).toString());
            list.push_back(u);
        }
    }
    return list;
}

bool UserRepository::create(User& user) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "INSERT INTO users (username, password_hash, salt, full_name, email, phone, role_id, is_active) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?);"
    );
    query.addBindValue(user.getUsername());
    query.addBindValue(user.getPasswordHash());
    query.addBindValue(user.getSalt());
    query.addBindValue(user.getFullName());
    query.addBindValue(user.getEmail());
    query.addBindValue(user.getPhone());
    query.addBindValue(User::roleToInt(user.getRole()));
    query.addBindValue(user.isActive() ? 1 : 0);

    if (query.exec()) {
        user.setId(query.lastInsertId().toInt());
        return true;
    }
    qWarning() << "UserRepository::create error:" << query.lastError().text();
    return false;
}

bool UserRepository::update(const User& user) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "UPDATE users SET full_name = ?, email = ?, phone = ?, role_id = ?, is_active = ?, updated_at = CURRENT_TIMESTAMP "
        "WHERE user_id = ?;"
    );
    query.addBindValue(user.getFullName());
    query.addBindValue(user.getEmail());
    query.addBindValue(user.getPhone());
    query.addBindValue(User::roleToInt(user.getRole()));
    query.addBindValue(user.isActive() ? 1 : 0);
    query.addBindValue(user.getId());

    return query.exec();
}

bool UserRepository::remove(int id) {
    QSqlQuery query = DatabaseManager::instance().prepare("DELETE FROM users WHERE user_id = ?;");
    query.addBindValue(id);
    return query.exec();
}

bool UserRepository::updatePassword(int userId, const QString& newHash, const QString& newSalt) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "UPDATE users SET password_hash = ?, salt = ?, updated_at = CURRENT_TIMESTAMP WHERE user_id = ?;"
    );
    query.addBindValue(newHash);
    query.addBindValue(newSalt);
    query.addBindValue(userId);
    return query.exec();
}

} // namespace FitCore
