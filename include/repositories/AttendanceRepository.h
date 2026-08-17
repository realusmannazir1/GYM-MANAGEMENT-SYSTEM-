#ifndef ATTENDANCEREPOSITORY_H
#define ATTENDANCEREPOSITORY_H

#include "models/Attendance.h"
#include <optional>
#include <vector>
#include <map>

namespace FitCore {

class AttendanceRepository {
public:
    AttendanceRepository() = default;

    std::optional<Attendance> findById(int id);
    std::optional<Attendance> findActiveCheckInByMemberId(int memberId);
    std::vector<Attendance> findByMemberId(int memberId);
    std::vector<Attendance> findByDate(const QString& date);
    std::vector<Attendance> findByDateRange(const QString& startDate, const QString& endDate);
    std::vector<Attendance> findAllPaged(int limit, int offset);

    bool checkIn(int memberId, int recordedByUserId, Attendance& outAttendance);
    bool checkOut(int attendanceId, const QString& checkOutTime = "");
    int getTodayVisitsCount();
    int getMonthlyVisitsCount();
    std::map<int, int> getPeakHoursStatistics();
};

} // namespace FitCore

#endif // ATTENDANCEREPOSITORY_H
