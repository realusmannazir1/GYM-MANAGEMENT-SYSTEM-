#include "models/ProgressRecord.h"

namespace FitCore {

ProgressRecord::ProgressRecord()
    : m_id(0), m_memberId(0), m_weightKg(0.0), m_heightCm(0.0), m_bmi(0.0),
      m_bodyFatPercentage(0.0), m_chestCm(0.0), m_waistCm(0.0),
      m_armsCm(0.0), m_thighsCm(0.0), m_shouldersCm(0.0) {}

int ProgressRecord::getId() const { return m_id; }
void ProgressRecord::setId(int id) { m_id = id; }

int ProgressRecord::getMemberId() const { return m_memberId; }
void ProgressRecord::setMemberId(int memberId) { m_memberId = memberId; }

QString ProgressRecord::getMemberName() const { return m_memberName; }
void ProgressRecord::setMemberName(const QString& name) { m_memberName = name; }

QString ProgressRecord::getRecordDate() const { return m_recordDate; }
void ProgressRecord::setRecordDate(const QString& date) { m_recordDate = date; }

double ProgressRecord::getWeightKg() const { return m_weightKg; }
void ProgressRecord::setWeightKg(double weight) {
    m_weightKg = weight;
    m_bmi = calculateBmi();
}

double ProgressRecord::getHeightCm() const { return m_heightCm; }
void ProgressRecord::setHeightCm(double height) {
    m_heightCm = height;
    m_bmi = calculateBmi();
}

double ProgressRecord::getBmi() const {
    if (m_bmi <= 0.0) return calculateBmi();
    return m_bmi;
}
void ProgressRecord::setBmi(double bmi) { m_bmi = bmi; }

double ProgressRecord::getBodyFatPercentage() const { return m_bodyFatPercentage; }
void ProgressRecord::setBodyFatPercentage(double fat) { m_bodyFatPercentage = fat; }

double ProgressRecord::getChestCm() const { return m_chestCm; }
void ProgressRecord::setChestCm(double chest) { m_chestCm = chest; }

double ProgressRecord::getWaistCm() const { return m_waistCm; }
void ProgressRecord::setWaistCm(double waist) { m_waistCm = waist; }

double ProgressRecord::getArmsCm() const { return m_armsCm; }
void ProgressRecord::setArmsCm(double arms) { m_armsCm = arms; }

double ProgressRecord::getThighsCm() const { return m_thighsCm; }
void ProgressRecord::setThighsCm(double thighs) { m_thighsCm = thighs; }

double ProgressRecord::getShouldersCm() const { return m_shouldersCm; }
void ProgressRecord::setShouldersCm(double shoulders) { m_shouldersCm = shoulders; }

QString ProgressRecord::getNotes() const { return m_notes; }
void ProgressRecord::setNotes(const QString& notes) { m_notes = notes; }

double ProgressRecord::calculateBmi() const {
    if (m_heightCm <= 0.0 || m_weightKg <= 0.0) return 0.0;
    double heightMeters = m_heightCm / 100.0;
    return m_weightKg / (heightMeters * heightMeters);
}

QString ProgressRecord::getBmiCategory() const {
    double val = getBmi();
    if (val <= 0.0) return "Unknown";
    if (val < 18.5) return "Underweight";
    if (val < 25.0) return "Normal";
    if (val < 30.0) return "Overweight";
    return "Obese";
}

} // namespace FitCore
