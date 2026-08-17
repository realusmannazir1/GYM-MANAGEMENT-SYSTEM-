#include "models/Attendance.h"

namespace FitCore {

Attendance::Attendance()
    : m_id(0), m_memberId(0), m_recordedBy(1) {}

int Attendance::getId() const { return m_id; }
void Attendance::setId(int id) { m_id = id; }

int Attendance::getMemberId() const { return m_memberId; }
void Attendance::setMemberId(int memberId) { m_memberId = memberId; }

QString Attendance::getMemberName() const { return m_memberName; }
void Attendance::setMemberName(const QString& name) { m_memberName = name; }

QString Attendance::getMembershipNumber() const { return m_membershipNumber; }
void Attendance::setMembershipNumber(const QString& number) { m_membershipNumber = number; }

QString Attendance::getAttendanceDate() const { return m_attendanceDate; }
void Attendance::setAttendanceDate(const QString& date) { m_attendanceDate = date; }

QString Attendance::getCheckInTime() const { return m_checkInTime; }
void Attendance::setCheckInTime(const QString& time) { m_checkInTime = time; }

QString Attendance::getCheckOutTime() const { return m_checkOutTime; }
void Attendance::setCheckOutTime(const QString& time) { m_checkOutTime = time; }

int Attendance::getRecordedBy() const { return m_recordedBy; }
void Attendance::setRecordedBy(int userId) { m_recordedBy = userId; }

bool Attendance::isCheckedOut() const {
    return !m_checkOutTime.isEmpty();
}

QString Attendance::getDurationFormatted() const {
    if (m_checkInTime.isEmpty() || m_checkOutTime.isEmpty()) {
        return "Active Session";
    }

    QTime tIn = QTime::fromString(m_checkInTime, "hh:mm:ss");
    if (!tIn.isValid()) tIn = QTime::fromString(m_checkInTime, "hh:mm");

    QTime tOut = QTime::fromString(m_checkOutTime, "hh:mm:ss");
    if (!tOut.isValid()) tOut = QTime::fromString(m_checkOutTime, "hh:mm");

    if (!tIn.isValid() || !tOut.isValid()) {
        return "--";
    }

    int seconds = tIn.secsTo(tOut);
    if (seconds < 0) seconds += 86400; // Over Midnight wrap

    int hours = seconds / 3600;
    int mins = (seconds % 3600) / 60;

    if (hours > 0) {
        return QString("%1h %2m").arg(hours).arg(mins);
    }
    return QString("%1m").arg(mins);
}

} // namespace FitCore
