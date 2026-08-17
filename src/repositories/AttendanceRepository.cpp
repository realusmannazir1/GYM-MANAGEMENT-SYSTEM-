#include "repositories/AttendanceRepository.h"
#include "database/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include <QDate>
#include <QTime>
#include <QDebug>

namespace FitCore {

static Attendance mapAttendance(const QSqlQuery& query) {
    Attendance a;
    a.setId(query.value("attendance_id").toInt());
    a.setMemberId(query.value("member_id").toInt());
    a.setAttendanceDate(query.value("attendance_date").toString());
    a.setCheckInTime(query.value("check_in_time").toString());
    a.setCheckOutTime(query.value("check_out_time").toString());
    a.setRecordedBy(query.value("recorded_by").toInt());

    if (query.record().indexOf("member_name") >= 0) {
        a.setMemberName(query.value("member_name").toString());
    }
    if (query.record().indexOf("membership_number") >= 0) {
        a.setMembershipNumber(query.value("membership_number").toString());
    }
    return a;
}

std::optional<Attendance> AttendanceRepository::findById(int id) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT a.*, (m.first_name || ' ' || m.last_name) AS member_name, m.membership_number "
        "FROM attendance a JOIN members m ON a.member_id = m.member_id WHERE a.attendance_id = ?;"
    );
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        return mapAttendance(query);
    }
    return std::nullopt;
}

std::optional<Attendance> AttendanceRepository::findActiveCheckInByMemberId(int memberId) {
    QString today = QDate::currentDate().toString("yyyy-MM-dd");
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT a.*, (m.first_name || ' ' || m.last_name) AS member_name, m.membership_number "
        "FROM attendance a JOIN members m ON a.member_id = m.member_id "
        "WHERE a.member_id = ? AND a.attendance_date = ? AND (a.check_out_time IS NULL OR a.check_out_time = '') "
        "ORDER BY a.attendance_id DESC LIMIT 1;"
    );
    query.addBindValue(memberId);
    query.addBindValue(today);

    if (query.exec() && query.next()) {
        return mapAttendance(query);
    }
    return std::nullopt;
}

std::vector<Attendance> AttendanceRepository::findByMemberId(int memberId) {
    std::vector<Attendance> list;
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT a.*, (m.first_name || ' ' || m.last_name) AS member_name, m.membership_number "
        "FROM attendance a JOIN members m ON a.member_id = m.member_id "
        "WHERE a.member_id = ? ORDER BY a.attendance_id DESC;"
    );
    query.addBindValue(memberId);
    if (query.exec()) {
        while (query.next()) list.push_back(mapAttendance(query));
    }
    return list;
}

std::vector<Attendance> AttendanceRepository::findByDate(const QString& date) {
    std::vector<Attendance> list;
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT a.*, (m.first_name || ' ' || m.last_name) AS member_name, m.membership_number "
        "FROM attendance a JOIN members m ON a.member_id = m.member_id "
        "WHERE a.attendance_date = ? ORDER BY a.check_in_time DESC;"
    );
    query.addBindValue(date);
    if (query.exec()) {
        while (query.next()) list.push_back(mapAttendance(query));
    }
    return list;
}

std::vector<Attendance> AttendanceRepository::findByDateRange(const QString& startDate, const QString& endDate) {
    std::vector<Attendance> list;
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT a.*, (m.first_name || ' ' || m.last_name) AS member_name, m.membership_number "
        "FROM attendance a JOIN members m ON a.member_id = m.member_id "
        "WHERE a.attendance_date >= ? AND a.attendance_date <= ? "
        "ORDER BY a.attendance_date DESC, a.check_in_time DESC;"
    );
    query.addBindValue(startDate);
    query.addBindValue(endDate);
    if (query.exec()) {
        while (query.next()) list.push_back(mapAttendance(query));
    }
    return list;
}

std::vector<Attendance> AttendanceRepository::findAllPaged(int limit, int offset) {
    std::vector<Attendance> list;
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT a.*, (m.first_name || ' ' || m.last_name) AS member_name, m.membership_number "
        "FROM attendance a JOIN members m ON a.member_id = m.member_id "
        "ORDER BY a.attendance_id DESC LIMIT ? OFFSET ?;"
    );
    query.addBindValue(limit);
    query.addBindValue(offset);
    if (query.exec()) {
        while (query.next()) list.push_back(mapAttendance(query));
    }
    return list;
}

bool AttendanceRepository::checkIn(int memberId, int recordedByUserId, Attendance& outAttendance) {
    QString today = QDate::currentDate().toString("yyyy-MM-dd");
    QString nowTime = QTime::currentTime().toString("hh:mm:ss");

    QSqlQuery query = DatabaseManager::instance().prepare(
        "INSERT INTO attendance (member_id, attendance_date, check_in_time, recorded_by) "
        "VALUES (?, ?, ?, ?);"
    );
    query.addBindValue(memberId);
    query.addBindValue(today);
    query.addBindValue(nowTime);
    query.addBindValue(recordedByUserId);

    if (query.exec()) {
        int id = query.lastInsertId().toInt();
        auto opt = findById(id);
        if (opt.has_value()) {
            outAttendance = opt.value();
        }
        return true;
    }
    qWarning() << "AttendanceRepository::checkIn error:" << query.lastError().text();
    return false;
}

bool AttendanceRepository::checkOut(int attendanceId, const QString& checkOutTime) {
    QString nowTime = checkOutTime.isEmpty() ? QTime::currentTime().toString("hh:mm:ss") : checkOutTime;
    QSqlQuery query = DatabaseManager::instance().prepare(
        "UPDATE attendance SET check_out_time = ? WHERE attendance_id = ?;"
    );
    query.addBindValue(nowTime);
    query.addBindValue(attendanceId);
    return query.exec();
}

int AttendanceRepository::getTodayVisitsCount() {
    QString today = QDate::currentDate().toString("yyyy-MM-dd");
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT COUNT(*) FROM attendance WHERE attendance_date = ?;"
    );
    query.addBindValue(today);
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

int AttendanceRepository::getMonthlyVisitsCount() {
    QString monthPrefix = QDate::currentDate().toString("yyyy-MM-");
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT COUNT(*) FROM attendance WHERE attendance_date LIKE ?;"
    );
    query.addBindValue(monthPrefix + "%");
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

std::map<int, int> AttendanceRepository::getPeakHoursStatistics() {
    std::map<int, int> peakHours;
    QSqlQuery query(
        "SELECT CAST(SUBSTR(check_in_time, 1, 2) AS INTEGER) AS hr, COUNT(*) AS cnt "
        "FROM attendance GROUP BY hr ORDER BY hr ASC;",
        DatabaseManager::instance().getDatabase()
    );
    while (query.next()) {
        peakHours[query.value("hr").toInt()] = query.value("cnt").toInt();
    }
    return peakHours;
}

} // namespace FitCore
