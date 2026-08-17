#include "repositories/MembershipRepository.h"
#include "database/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDate>
#include <QDebug>

namespace FitCore {

static Membership mapMembership(const QSqlQuery& query) {
    Membership m;
    m.setId(query.value("membership_id").toInt());
    m.setMemberId(query.value("member_id").toInt());
    m.setPlanId(query.value("plan_id").toInt());
    m.setStartDate(query.value("start_date").toString());
    m.setEndDate(query.value("end_date").toString());
    m.setPrice(query.value("price").toDouble());
    m.setDiscount(query.value("discount").toDouble());
    m.setFinalAmount(query.value("final_amount").toDouble());
    m.setStatus(query.value("status").toString());
    m.setPaymentStatus(query.value("payment_status").toString());
    m.setCreatedBy(query.value("created_by").toInt());

    if (query.record().indexOf("plan_name") >= 0) {
        m.setPlanName(query.value("plan_name").toString());
    }
    if (query.record().indexOf("member_name") >= 0) {
        m.setMemberName(query.value("member_name").toString());
    }
    return m;
}

std::optional<Membership> MembershipRepository::findById(int id) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT ms.*, p.plan_name, (m.first_name || ' ' || m.last_name) AS member_name "
        "FROM memberships ms "
        "JOIN membership_plans p ON ms.plan_id = p.plan_id "
        "JOIN members m ON ms.member_id = m.member_id "
        "WHERE ms.membership_id = ?;"
    );
    query.addBindValue(id);

    if (query.exec() && query.next()) {
        return mapMembership(query);
    }
    return std::nullopt;
}

std::optional<Membership> MembershipRepository::findActiveByMemberId(int memberId) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT ms.*, p.plan_name, (m.first_name || ' ' || m.last_name) AS member_name "
        "FROM memberships ms "
        "JOIN membership_plans p ON ms.plan_id = p.plan_id "
        "JOIN members m ON ms.member_id = m.member_id "
        "WHERE ms.member_id = ? AND ms.status = 'Active' "
        "ORDER BY ms.end_date DESC LIMIT 1;"
    );
    query.addBindValue(memberId);

    if (query.exec() && query.next()) {
        return mapMembership(query);
    }
    return std::nullopt;
}

std::vector<Membership> MembershipRepository::findHistoryByMemberId(int memberId) {
    std::vector<Membership> list;
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT ms.*, p.plan_name, (m.first_name || ' ' || m.last_name) AS member_name "
        "FROM memberships ms "
        "JOIN membership_plans p ON ms.plan_id = p.plan_id "
        "JOIN members m ON ms.member_id = m.member_id "
        "WHERE ms.member_id = ? ORDER BY ms.membership_id DESC;"
    );
    query.addBindValue(memberId);

    if (query.exec()) {
        while (query.next()) {
            list.push_back(mapMembership(query));
        }
    }
    return list;
}

std::vector<Membership> MembershipRepository::findAll(const QString& statusFilter) {
    std::vector<Membership> list;
    QString sql = "SELECT ms.*, p.plan_name, (m.first_name || ' ' || m.last_name) AS member_name "
                  "FROM memberships ms "
                  "JOIN membership_plans p ON ms.plan_id = p.plan_id "
                  "JOIN members m ON ms.member_id = m.member_id ";
    if (!statusFilter.isEmpty() && statusFilter != "All") {
        sql += "WHERE ms.status = '" + statusFilter + "' ";
    }
    sql += "ORDER BY ms.membership_id DESC;";

    QSqlQuery query(sql, DatabaseManager::instance().getDatabase());
    while (query.next()) {
        list.push_back(mapMembership(query));
    }
    return list;
}

std::vector<Membership> MembershipRepository::findExpiringSoon(int withinDays) {
    std::vector<Membership> list;
    QString today = QDate::currentDate().toString("yyyy-MM-dd");
    QString targetDate = QDate::currentDate().addDays(withinDays).toString("yyyy-MM-dd");

    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT ms.*, p.plan_name, (m.first_name || ' ' || m.last_name) AS member_name "
        "FROM memberships ms "
        "JOIN membership_plans p ON ms.plan_id = p.plan_id "
        "JOIN members m ON ms.member_id = m.member_id "
        "WHERE ms.status = 'Active' AND ms.end_date >= ? AND ms.end_date <= ? "
        "ORDER BY ms.end_date ASC;"
    );
    query.addBindValue(today);
    query.addBindValue(targetDate);

    if (query.exec()) {
        while (query.next()) {
            list.push_back(mapMembership(query));
        }
    }
    return list;
}

std::vector<Membership> MembershipRepository::findExpired() {
    std::vector<Membership> list;
    QString today = QDate::currentDate().toString("yyyy-MM-dd");

    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT ms.*, p.plan_name, (m.first_name || ' ' || m.last_name) AS member_name "
        "FROM memberships ms "
        "JOIN membership_plans p ON ms.plan_id = p.plan_id "
        "JOIN members m ON ms.member_id = m.member_id "
        "WHERE (ms.status = 'Expired' OR (ms.status = 'Active' AND ms.end_date < ?)) "
        "ORDER BY ms.end_date DESC;"
    );
    query.addBindValue(today);

    if (query.exec()) {
        while (query.next()) {
            list.push_back(mapMembership(query));
        }
    }
    return list;
}

bool MembershipRepository::create(Membership& membership) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "INSERT INTO memberships (member_id, plan_id, start_date, end_date, price, discount, final_amount, status, payment_status, created_by) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"
    );
    query.addBindValue(membership.getMemberId());
    query.addBindValue(membership.getPlanId());
    query.addBindValue(membership.getStartDate());
    query.addBindValue(membership.getEndDate());
    query.addBindValue(membership.getPrice());
    query.addBindValue(membership.getDiscount());
    query.addBindValue(membership.getFinalAmount());
    query.addBindValue(membership.getStatus());
    query.addBindValue(membership.getPaymentStatus());
    query.addBindValue(membership.getCreatedBy());

    if (query.exec()) {
        membership.setId(query.lastInsertId().toInt());
        return true;
    }
    qWarning() << "MembershipRepository::create error:" << query.lastError().text();
    return false;
}

bool MembershipRepository::update(const Membership& membership) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "UPDATE memberships SET plan_id = ?, start_date = ?, end_date = ?, price = ?, discount = ?, "
        "final_amount = ?, status = ?, payment_status = ? WHERE membership_id = ?;"
    );
    query.addBindValue(membership.getPlanId());
    query.addBindValue(membership.getStartDate());
    query.addBindValue(membership.getEndDate());
    query.addBindValue(membership.getPrice());
    query.addBindValue(membership.getDiscount());
    query.addBindValue(membership.getFinalAmount());
    query.addBindValue(membership.getStatus());
    query.addBindValue(membership.getPaymentStatus());
    query.addBindValue(membership.getId());
    return query.exec();
}

bool MembershipRepository::setStatus(int membershipId, const QString& status) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "UPDATE memberships SET status = ? WHERE membership_id = ?;"
    );
    query.addBindValue(status);
    query.addBindValue(membershipId);
    return query.exec();
}

bool MembershipRepository::autoUpdateExpiredStatuses() {
    QString today = QDate::currentDate().toString("yyyy-MM-dd");
    QSqlQuery query = DatabaseManager::instance().prepare(
        "UPDATE memberships SET status = 'Expired' WHERE status = 'Active' AND end_date < ?;"
    );
    query.addBindValue(today);
    return query.exec();
}

} // namespace FitCore
