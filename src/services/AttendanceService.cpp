#include "services/AttendanceService.h"
#include <QDate>
#include <QTime>
#include <QDebug>

namespace FitCore {

CheckInResult AttendanceService::checkInByMembershipNumber(const QString& membershipNumber, int recordedByUserId) {
    MemberRepository memberRepo;
    auto opt = memberRepo.findByMembershipNumber(membershipNumber);
    if (!opt.has_value()) {
        CheckInResult res;
        res.message = "Member with membership number '" + membershipNumber + "' not found.";
        return res;
    }
    return checkInByMemberId(opt->getId(), recordedByUserId);
}

CheckInResult AttendanceService::checkInByMemberId(int memberId, int recordedByUserId) {
    CheckInResult res;
    MemberRepository memberRepo;
    MembershipRepository membershipRepo;
    AttendanceRepository attendanceRepo;
    AuditRepository auditRepo;

    // 1. Member existence check
    auto memberOpt = memberRepo.findById(memberId);
    if (!memberOpt.has_value()) {
        res.message = "Member record does not exist.";
        return res;
    }

    Member member = memberOpt.value();

    // 2. Profile active check
    if (member.getProfileStatus() != "Active") {
        res.message = QString("Cannot check in. Member profile status is '%1'.").arg(member.getProfileStatus());
        return res;
    }

    // 3. Active membership & expiry check
    auto msOpt = membershipRepo.findActiveByMemberId(memberId);
    if (!msOpt.has_value()) {
        res.message = "Cannot check in. Member has no active membership plan.";
        return res;
    }

    Membership ms = msOpt.value();
    QDate endDate = QDate::fromString(ms.getEndDate(), "yyyy-MM-dd");
    QDate today = QDate::currentDate();

    if (endDate.isValid() && endDate < today) {
        res.message = QString("Cannot check in. Membership expired on %1.").arg(endDate.toString("dd-MMM-yyyy"));
        return res;
    }

    // 4. Duplicate check-in validation
    auto activeCheckIn = attendanceRepo.findActiveCheckInByMemberId(memberId);
    if (activeCheckIn.has_value()) {
        res.message = QString("Member %1 is already checked in at %2.")
                          .arg(member.getFullName(), activeCheckIn->getCheckInTime());
        return res;
    }

    // 5. Execute Check-in
    Attendance att;
    if (attendanceRepo.checkIn(memberId, recordedByUserId, att)) {
        res.success = true;
        res.attendance = att;
        res.message = QString("Welcome %1! Check-in recorded at %2.")
                          .arg(member.getFullName(), att.getCheckInTime());

        auditRepo.logAction(recordedByUserId, "CHECK_IN", "Attendance", att.getId(),
                            "Checked in member " + member.getFullName() + " (" + member.getMembershipNumber() + ")");
        return res;
    }

    res.message = "Failed to insert attendance check-in record into database.";
    return res;
}

bool AttendanceService::checkOut(int attendanceId, int currentUserId) {
    AttendanceRepository repo;
    AuditRepository auditRepo;

    auto attOpt = repo.findById(attendanceId);
    if (!attOpt.has_value()) {
        return false;
    }

    if (repo.checkOut(attendanceId)) {
        auditRepo.logAction(currentUserId, "CHECK_OUT", "Attendance", attendanceId,
                            "Checked out member ID " + QString::number(attOpt->getMemberId()));
        return true;
    }
    return false;
}

} // namespace FitCore
