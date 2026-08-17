#include "models/Trainer.h"

namespace FitCore {

Trainer::Trainer()
    : m_id(0), m_experienceYears(0), m_salary(0.0), m_status("Active") {}

int Trainer::getId() const { return m_id; }
void Trainer::setId(int id) { m_id = id; }

QString Trainer::getEmployeeNumber() const { return m_employeeNumber; }
void Trainer::setEmployeeNumber(const QString& number) { m_employeeNumber = number; }

QString Trainer::getFirstName() const { return m_firstName; }
void Trainer::setFirstName(const QString& name) { m_firstName = name; }

QString Trainer::getLastName() const { return m_lastName; }
void Trainer::setLastName(const QString& name) { m_lastName = name; }

QString Trainer::getFullName() const {
    if (m_firstName.isEmpty()) return m_lastName;
    if (m_lastName.isEmpty()) return m_firstName;
    return m_firstName + " " + m_lastName;
}

QString Trainer::getPhone() const { return m_phone; }
void Trainer::setPhone(const QString& phone) { m_phone = phone; }

QString Trainer::getEmail() const { return m_email; }
void Trainer::setEmail(const QString& email) { m_email = email; }

QString Trainer::getSpecialization() const { return m_specialization; }
void Trainer::setSpecialization(const QString& spec) { m_specialization = spec; }

int Trainer::getExperienceYears() const { return m_experienceYears; }
void Trainer::setExperienceYears(int years) { m_experienceYears = years; }

double Trainer::getSalary() const { return m_salary; }
void Trainer::setSalary(double salary) { m_salary = salary; }

QString Trainer::getJoiningDate() const { return m_joiningDate; }
void Trainer::setJoiningDate(const QString& date) { m_joiningDate = date; }

QString Trainer::getStatus() const { return m_status; }
void Trainer::setStatus(const QString& status) { m_status = status; }

} // namespace FitCore
