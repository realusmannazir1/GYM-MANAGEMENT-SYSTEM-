#ifndef ATTENDANCESERVICE_H
#define ATTENDANCESERVICE_H

#include "models/Attendance.h"
#include "repositories/AttendanceRepository.h"
#include "repositories/MemberRepository.h"
#include "repositories/MembershipRepository.h"
#include "repositories/AuditRepository.h"
#include <QString>

namespace FitCore {

struct CheckInResult {
    bool success{false};
    QString message;
    Attendance attendance;
};

class AttendanceService {
public:
    AttendanceService() = default;

    CheckInResult checkInByMembershipNumber(const QString& membershipNumber, int recordedByUserId);
    CheckInResult checkInByMemberId(int memberId, int recordedByUserId);
    bool checkOut(int attendanceId, int currentUserId);
};

} // namespace FitCore

#endif // ATTENDANCESERVICE_H
