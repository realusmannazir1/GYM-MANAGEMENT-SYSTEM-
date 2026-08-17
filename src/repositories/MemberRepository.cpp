#include "repositories/MemberRepository.h"
#include "database/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

namespace FitCore {

static Member mapMember(const QSqlQuery& query) {
    Member m;
    m.setId(query.value("member_id").toInt());
    m.setMembershipNumber(query.value("membership_number").toString());
    m.setFirstName(query.value("first_name").toString());
    m.setLastName(query.value("last_name").toString());
    m.setGender(query.value("gender").toString());
    m.setDateOfBirth(query.value("date_of_birth").toString());
    m.setPhone(query.value("phone").toString());
    m.setEmail(query.value("email").toString());
    m.setAddress(query.value("address").toString());
    m.setEmergencyContact(query.value("emergency_contact").toString());
    m.setEmergencyPhone(query.value("emergency_phone").toString());
    m.setRegistrationDate(query.value("registration_date").toString());
    m.setProfileStatus(query.value("profile_status").toString());
    m.setPhotoPath(query.value("photo_path").toString());
    m.setNotes(query.value("notes").toString());
    m.setCreatedAt(query.value("created_at").toString());
    m.setUpdatedAt(query.value("updated_at").toString());
    return m;
}

std::optional<Member> MemberRepository::findById(int id) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT * FROM members WHERE member_id = ?;"
    );
    query.addBindValue(id);

    if (query.exec() && query.next()) {
        return mapMember(query);
    }
    return std::nullopt;
}

std::optional<Member> MemberRepository::findByMembershipNumber(const QString& number) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT * FROM members WHERE UPPER(membership_number) = UPPER(?);"
    );
    query.addBindValue(number.trimmed());

    if (query.exec() && query.next()) {
        return mapMember(query);
    }
    return std::nullopt;
}

std::vector<Member> MemberRepository::findAll() {
    std::vector<Member> list;
    QSqlQuery query("SELECT * FROM members ORDER BY member_id DESC;", DatabaseManager::instance().getDatabase());
    while (query.next()) {
        list.push_back(mapMember(query));
    }
    return list;
}

std::vector<Member> MemberRepository::findAllPaged(int limit, int offset, const QString& statusFilter) {
    std::vector<Member> list;
    QString sql = "SELECT * FROM members ";
    if (!statusFilter.isEmpty() && statusFilter != "All") {
        sql += "WHERE profile_status = '" + statusFilter + "' ";
    }
    sql += "ORDER BY member_id DESC LIMIT ? OFFSET ?;";

    QSqlQuery query = DatabaseManager::instance().prepare(sql);
    query.addBindValue(limit);
    query.addBindValue(offset);

    if (query.exec()) {
        while (query.next()) {
            list.push_back(mapMember(query));
        }
    }
    return list;
}

std::vector<Member> MemberRepository::search(const QString& keyword) {
    std::vector<Member> list;
    QString searchTerm = "%" + keyword.trimmed() + "%";
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT * FROM members WHERE first_name LIKE ? OR last_name LIKE ? "
        "OR membership_number LIKE ? OR phone LIKE ? OR email LIKE ? "
        "ORDER BY member_id DESC;"
    );
    query.addBindValue(searchTerm);
    query.addBindValue(searchTerm);
    query.addBindValue(searchTerm);
    query.addBindValue(searchTerm);
    query.addBindValue(searchTerm);

    if (query.exec()) {
        while (query.next()) {
            list.push_back(mapMember(query));
        }
    }
    return list;
}

int MemberRepository::count(const QString& statusFilter) {
    QString sql = "SELECT COUNT(*) FROM members";
    if (!statusFilter.isEmpty() && statusFilter != "All") {
        sql += " WHERE profile_status = '" + statusFilter + "'";
    }
    sql += ";";

    QSqlQuery query(sql, DatabaseManager::instance().getDatabase());
    if (query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

bool MemberRepository::create(Member& member) {
    if (member.getMembershipNumber().isEmpty()) {
        member.setMembershipNumber(generateNextMembershipNumber());
    }

    QSqlQuery query = DatabaseManager::instance().prepare(
        "INSERT INTO members (membership_number, first_name, last_name, gender, date_of_birth, "
        "phone, email, address, emergency_contact, emergency_phone, registration_date, profile_status, photo_path, notes) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"
    );
    query.addBindValue(member.getMembershipNumber());
    query.addBindValue(member.getFirstName());
    query.addBindValue(member.getLastName());
    query.addBindValue(member.getGender());
    query.addBindValue(member.getDateOfBirth());
    query.addBindValue(member.getPhone());
    query.addBindValue(member.getEmail());
    query.addBindValue(member.getAddress());
    query.addBindValue(member.getEmergencyContact());
    query.addBindValue(member.getEmergencyPhone());
    query.addBindValue(member.getRegistrationDate().isEmpty() ? QDate::currentDate().toString("yyyy-MM-dd") : member.getRegistrationDate());
    query.addBindValue(member.getProfileStatus());
    query.addBindValue(member.getPhotoPath());
    query.addBindValue(member.getNotes());

    if (query.exec()) {
        member.setId(query.lastInsertId().toInt());
        return true;
    }
    qWarning() << "MemberRepository::create error:" << query.lastError().text();
    return false;
}

bool MemberRepository::update(const Member& member) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "UPDATE members SET first_name = ?, last_name = ?, gender = ?, date_of_birth = ?, "
        "phone = ?, email = ?, address = ?, emergency_contact = ?, emergency_phone = ?, "
        "profile_status = ?, photo_path = ?, notes = ?, updated_at = CURRENT_TIMESTAMP "
        "WHERE member_id = ?;"
    );
    query.addBindValue(member.getFirstName());
    query.addBindValue(member.getLastName());
    query.addBindValue(member.getGender());
    query.addBindValue(member.getDateOfBirth());
    query.addBindValue(member.getPhone());
    query.addBindValue(member.getEmail());
    query.addBindValue(member.getAddress());
    query.addBindValue(member.getEmergencyContact());
    query.addBindValue(member.getEmergencyPhone());
    query.addBindValue(member.getProfileStatus());
    query.addBindValue(member.getPhotoPath());
    query.addBindValue(member.getNotes());
    query.addBindValue(member.getId());

    return query.exec();
}

bool MemberRepository::remove(int id) {
    // Soft delete to protect financial/attendance history
    return setStatus(id, "Archived");
}

bool MemberRepository::setStatus(int id, const QString& status) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "UPDATE members SET profile_status = ?, updated_at = CURRENT_TIMESTAMP WHERE member_id = ?;"
    );
    query.addBindValue(status);
    query.addBindValue(id);
    return query.exec();
}

QString MemberRepository::generateNextMembershipNumber() {
    QSqlQuery query("SELECT MAX(member_id) FROM members;", DatabaseManager::instance().getDatabase());
    int maxId = 0;
    if (query.next()) {
        maxId = query.value(0).toInt();
    }
    int nextId = maxId + 1;
    return QString("MEM-%1").arg(nextId, 6, 10, QChar('0'));
}

} // namespace FitCore
