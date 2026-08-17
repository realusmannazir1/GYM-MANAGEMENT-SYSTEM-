#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include <QString>
#include <QTime>

namespace FitCore {

class Attendance {
public:
    Attendance();

    int getId() const;
    void setId(int id);

    int getMemberId() const;
    void setMemberId(int memberId);

    QString getMemberName() const;
    void setMemberName(const QString& name);

    QString getMembershipNumber() const;
    void setMembershipNumber(const QString& number);

    QString getAttendanceDate() const;
    void setAttendanceDate(const QString& date);

    QString getCheckInTime() const;
    void setCheckInTime(const QString& time);

    QString getCheckOutTime() const;
    void setCheckOutTime(const QString& time);

    int getRecordedBy() const;
    void setRecordedBy(int userId);

    bool isCheckedOut() const;
    QString getDurationFormatted() const;

private:
    int m_id;
    int m_memberId;
    QString m_memberName;
    QString m_membershipNumber;
    QString m_attendanceDate;
    QString m_checkInTime;
    QString m_checkOutTime;
    int m_recordedBy;
};

} // namespace FitCore

#endif // ATTENDANCE_H
