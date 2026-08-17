#ifndef MEMBER_H
#define MEMBER_H

#include <QString>

namespace FitCore {

class Member {
public:
    Member();

    int getId() const;
    void setId(int id);

    QString getMembershipNumber() const;
    void setMembershipNumber(const QString& membershipNumber);

    QString getFirstName() const;
    void setFirstName(const QString& firstName);

    QString getLastName() const;
    void setLastName(const QString& lastName);

    QString getFullName() const;

    QString getGender() const;
    void setGender(const QString& gender);

    QString getDateOfBirth() const;
    void setDateOfBirth(const QString& dob);

    QString getPhone() const;
    void setPhone(const QString& phone);

    QString getEmail() const;
    void setEmail(const QString& email);

    QString getAddress() const;
    void setAddress(const QString& address);

    QString getEmergencyContact() const;
    void setEmergencyContact(const QString& contact);

    QString getEmergencyPhone() const;
    void setEmergencyPhone(const QString& phone);

    QString getRegistrationDate() const;
    void setRegistrationDate(const QString& date);

    QString getProfileStatus() const;
    void setProfileStatus(const QString& status);

    QString getPhotoPath() const;
    void setPhotoPath(const QString& path);

    QString getNotes() const;
    void setNotes(const QString& notes);

    QString getCreatedAt() const;
    void setCreatedAt(const QString& createdAt);

    QString getUpdatedAt() const;
    void setUpdatedAt(const QString& updatedAt);

private:
    int m_id;
    QString m_membershipNumber;
    QString m_firstName;
    QString m_lastName;
    QString m_gender;
    QString m_dateOfBirth;
    QString m_phone;
    QString m_email;
    QString m_address;
    QString m_emergencyContact;
    QString m_emergencyPhone;
    QString m_registrationDate;
    QString m_profileStatus;
    QString m_photoPath;
    QString m_notes;
    QString m_createdAt;
    QString m_updatedAt;
};

} // namespace FitCore

#endif // MEMBER_H
