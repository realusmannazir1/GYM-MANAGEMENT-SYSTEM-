#include "models/Member.h"

namespace FitCore {

Member::Member()
    : m_id(0), m_profileStatus("Active") {}

int Member::getId() const { return m_id; }
void Member::setId(int id) { m_id = id; }

QString Member::getMembershipNumber() const { return m_membershipNumber; }
void Member::setMembershipNumber(const QString& membershipNumber) { m_membershipNumber = membershipNumber; }

QString Member::getFirstName() const { return m_firstName; }
void Member::setFirstName(const QString& firstName) { m_firstName = firstName; }

QString Member::getLastName() const { return m_lastName; }
void Member::setLastName(const QString& lastName) { m_lastName = lastName; }

QString Member::getFullName() const {
    if (m_firstName.isEmpty()) return m_lastName;
    if (m_lastName.isEmpty()) return m_firstName;
    return m_firstName + " " + m_lastName;
}

QString Member::getGender() const { return m_gender; }
void Member::setGender(const QString& gender) { m_gender = gender; }

QString Member::getDateOfBirth() const { return m_dateOfBirth; }
void Member::setDateOfBirth(const QString& dob) { m_dateOfBirth = dob; }

QString Member::getPhone() const { return m_phone; }
void Member::setPhone(const QString& phone) { m_phone = phone; }

QString Member::getEmail() const { return m_email; }
void Member::setEmail(const QString& email) { m_email = email; }

QString Member::getAddress() const { return m_address; }
void Member::setAddress(const QString& address) { m_address = address; }

QString Member::getEmergencyContact() const { return m_emergencyContact; }
void Member::setEmergencyContact(const QString& contact) { m_emergencyContact = contact; }

QString Member::getEmergencyPhone() const { return m_emergencyPhone; }
void Member::setEmergencyPhone(const QString& phone) { m_emergencyPhone = phone; }

QString Member::getRegistrationDate() const { return m_registrationDate; }
void Member::setRegistrationDate(const QString& date) { m_registrationDate = date; }

QString Member::getProfileStatus() const { return m_profileStatus; }
void Member::setProfileStatus(const QString& status) { m_profileStatus = status; }

QString Member::getPhotoPath() const { return m_photoPath; }
void Member::setPhotoPath(const QString& path) { m_photoPath = path; }

QString Member::getNotes() const { return m_notes; }
void Member::setNotes(const QString& notes) { m_notes = notes; }

QString Member::getCreatedAt() const { return m_createdAt; }
void Member::setCreatedAt(const QString& createdAt) { m_createdAt = createdAt; }

QString Member::getUpdatedAt() const { return m_updatedAt; }
void Member::setUpdatedAt(const QString& updatedAt) { m_updatedAt = updatedAt; }

} // namespace FitCore
