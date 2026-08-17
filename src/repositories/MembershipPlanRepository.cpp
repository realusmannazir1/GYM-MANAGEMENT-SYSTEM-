#include "repositories/MembershipPlanRepository.h"
#include "database/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

namespace FitCore {

static MembershipPlan mapPlan(const QSqlQuery& query) {
    MembershipPlan p;
    p.setId(query.value("plan_id").toInt());
    p.setPlanName(query.value("plan_name").toString());
    p.setDurationDays(query.value("duration_days").toInt());
    p.setPrice(query.value("price").toDouble());
    p.setDescription(query.value("description").toString());
    p.setAccessType(query.value("access_type").toString());
    p.setStatus(query.value("status").toString());
    return p;
}

std::optional<MembershipPlan> MembershipPlanRepository::findById(int id) {
    QSqlQuery query = DatabaseManager::instance().prepare("SELECT * FROM membership_plans WHERE plan_id = ?;");
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        return mapPlan(query);
    }
    return std::nullopt;
}

std::vector<MembershipPlan> MembershipPlanRepository::findAll(bool activeOnly) {
    std::vector<MembershipPlan> list;
    QString sql = "SELECT * FROM membership_plans";
    if (activeOnly) {
        sql += " WHERE status = 'Active'";
    }
    sql += " ORDER BY duration_days ASC;";

    QSqlQuery query(sql, DatabaseManager::instance().getDatabase());
    while (query.next()) {
        list.push_back(mapPlan(query));
    }
    return list;
}

bool MembershipPlanRepository::create(MembershipPlan& plan) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "INSERT INTO membership_plans (plan_name, duration_days, price, description, access_type, status) "
        "VALUES (?, ?, ?, ?, ?, ?);"
    );
    query.addBindValue(plan.getPlanName());
    query.addBindValue(plan.getDurationDays());
    query.addBindValue(plan.getPrice());
    query.addBindValue(plan.getDescription());
    query.addBindValue(plan.getAccessType());
    query.addBindValue(plan.getStatus());

    if (query.exec()) {
        plan.setId(query.lastInsertId().toInt());
        return true;
    }
    return false;
}

bool MembershipPlanRepository::update(const MembershipPlan& plan) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "UPDATE membership_plans SET plan_name = ?, duration_days = ?, price = ?, "
        "description = ?, access_type = ?, status = ?, updated_at = CURRENT_TIMESTAMP "
        "WHERE plan_id = ?;"
    );
    query.addBindValue(plan.getPlanName());
    query.addBindValue(plan.getDurationDays());
    query.addBindValue(plan.getPrice());
    query.addBindValue(plan.getDescription());
    query.addBindValue(plan.getAccessType());
    query.addBindValue(plan.getStatus());
    query.addBindValue(plan.getId());
    return query.exec();
}

bool MembershipPlanRepository::setStatus(int id, const QString& status) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "UPDATE membership_plans SET status = ?, updated_at = CURRENT_TIMESTAMP WHERE plan_id = ?;"
    );
    query.addBindValue(status);
    query.addBindValue(id);
    return query.exec();
}

} // namespace FitCore
