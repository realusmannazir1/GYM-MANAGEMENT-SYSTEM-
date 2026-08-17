#include "repositories/ProgressRepository.h"
#include "database/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include <QDate>

namespace FitCore {

static ProgressRecord mapProgress(const QSqlQuery& query) {
    ProgressRecord p;
    p.setId(query.value("progress_id").toInt());
    p.setMemberId(query.value("member_id").toInt());
    p.setRecordDate(query.value("record_date").toString());
    p.setWeightKg(query.value("weight_kg").toDouble());
    p.setHeightCm(query.value("height_cm").toDouble());
    p.setBmi(query.value("bmi").toDouble());
    p.setBodyFatPercentage(query.value("body_fat_percentage").toDouble());
    p.setChestCm(query.value("chest_cm").toDouble());
    p.setWaistCm(query.value("waist_cm").toDouble());
    p.setArmsCm(query.value("arms_cm").toDouble());
    p.setThighsCm(query.value("thighs_cm").toDouble());
    p.setShouldersCm(query.value("shoulders_cm").toDouble());
    p.setNotes(query.value("notes").toString());

    if (query.record().indexOf("member_name") >= 0) {
        p.setMemberName(query.value("member_name").toString());
    }
    return p;
}

std::optional<ProgressRecord> ProgressRepository::findById(int id) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT pr.*, (m.first_name || ' ' || m.last_name) AS member_name "
        "FROM progress_records pr JOIN members m ON pr.member_id = m.member_id WHERE pr.progress_id = ?;"
    );
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        return mapProgress(query);
    }
    return std::nullopt;
}

std::vector<ProgressRecord> ProgressRepository::findByMemberId(int memberId) {
    std::vector<ProgressRecord> list;
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT pr.*, (m.first_name || ' ' || m.last_name) AS member_name "
        "FROM progress_records pr JOIN members m ON pr.member_id = m.member_id "
        "WHERE pr.member_id = ? ORDER BY pr.record_date ASC, pr.progress_id ASC;"
    );
    query.addBindValue(memberId);
    if (query.exec()) {
        while (query.next()) list.push_back(mapProgress(query));
    }
    return list;
}

std::optional<ProgressRecord> ProgressRepository::findLatestByMemberId(int memberId) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT pr.*, (m.first_name || ' ' || m.last_name) AS member_name "
        "FROM progress_records pr JOIN members m ON pr.member_id = m.member_id "
        "WHERE pr.member_id = ? ORDER BY pr.record_date DESC, pr.progress_id DESC LIMIT 1;"
    );
    query.addBindValue(memberId);
    if (query.exec() && query.next()) {
        return mapProgress(query);
    }
    return std::nullopt;
}

bool ProgressRepository::create(ProgressRecord& record) {
    if (record.getBmi() <= 0.0) {
        record.setBmi(record.calculateBmi());
    }

    QSqlQuery query = DatabaseManager::instance().prepare(
        "INSERT INTO progress_records (member_id, record_date, weight_kg, height_cm, bmi, body_fat_percentage, "
        "chest_cm, waist_cm, arms_cm, thighs_cm, shoulders_cm, notes) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"
    );
    query.addBindValue(record.getMemberId());
    query.addBindValue(record.getRecordDate().isEmpty() ? QDate::currentDate().toString("yyyy-MM-dd") : record.getRecordDate());
    query.addBindValue(record.getWeightKg());
    query.addBindValue(record.getHeightCm());
    query.addBindValue(record.getBmi());
    query.addBindValue(record.getBodyFatPercentage());
    query.addBindValue(record.getChestCm());
    query.addBindValue(record.getWaistCm());
    query.addBindValue(record.getArmsCm());
    query.addBindValue(record.getThighsCm());
    query.addBindValue(record.getShouldersCm());
    query.addBindValue(record.getNotes());

    if (query.exec()) {
        record.setId(query.lastInsertId().toInt());
        return true;
    }
    return false;
}

bool ProgressRepository::update(const ProgressRecord& record) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "UPDATE progress_records SET record_date = ?, weight_kg = ?, height_cm = ?, bmi = ?, "
        "body_fat_percentage = ?, chest_cm = ?, waist_cm = ?, arms_cm = ?, thighs_cm = ?, "
        "shoulders_cm = ?, notes = ? WHERE progress_id = ?;"
    );
    query.addBindValue(record.getRecordDate());
    query.addBindValue(record.getWeightKg());
    query.addBindValue(record.getHeightCm());
    query.addBindValue(record.getBmi());
    query.addBindValue(record.getBodyFatPercentage());
    query.addBindValue(record.getChestCm());
    query.addBindValue(record.getWaistCm());
    query.addBindValue(record.getArmsCm());
    query.addBindValue(record.getThighsCm());
    query.addBindValue(record.getShouldersCm());
    query.addBindValue(record.getNotes());
    query.addBindValue(record.getId());
    return query.exec();
}

bool ProgressRepository::remove(int id) {
    QSqlQuery query = DatabaseManager::instance().prepare("DELETE FROM progress_records WHERE progress_id = ?;");
    query.addBindValue(id);
    return query.exec();
}

} // namespace FitCore
