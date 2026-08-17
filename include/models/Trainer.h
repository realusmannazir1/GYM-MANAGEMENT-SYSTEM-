#ifndef TRAINER_H
#define TRAINER_H

#include <QString>
#include <QStringList>

namespace FitCore {

class Trainer {
public:
    Trainer();

    int getId() const;
    void setId(int id);

    QString getEmployeeNumber() const;
    void setEmployeeNumber(const QString& number);

    QString getFirstName() const;
    void setFirstName(const QString& name);

    QString getLastName() const;
    void setLastName(const QString& name);

    QString getFullName() const;

    QString getPhone() const;
    void setPhone(const QString& phone);

    QString getEmail() const;
    void setEmail(const QString& email);

    QString getSpecialization() const;
    void setSpecialization(const QString& spec);

    int getExperienceYears() const;
    void setExperienceYears(int years);

    double getSalary() const;
    void setSalary(double salary);

    QString getJoiningDate() const;
    void setJoiningDate(const QString& date);

    QString getStatus() const;
    void setStatus(const QString& status);

private:
    int m_id;
    QString m_employeeNumber;
    QString m_firstName;
    QString m_lastName;
    QString m_phone;
    QString m_email;
    QString m_specialization;
    int m_experienceYears;
    double m_salary;
    QString m_joiningDate;
    QString m_status;
};

} // namespace FitCore

#endif // TRAINER_H
