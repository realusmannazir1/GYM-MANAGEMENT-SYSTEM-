#ifndef PROGRESSRECORD_H
#define PROGRESSRECORD_H

#include <QString>

namespace FitCore {

class ProgressRecord {
public:
    ProgressRecord();

    int getId() const;
    void setId(int id);

    int getMemberId() const;
    void setMemberId(int memberId);

    QString getMemberName() const;
    void setMemberName(const QString& name);

    QString getRecordDate() const;
    void setRecordDate(const QString& date);

    double getWeightKg() const;
    void setWeightKg(double weight);

    double getHeightCm() const;
    void setHeightCm(double height);

    double getBmi() const;
    void setBmi(double bmi);

    double getBodyFatPercentage() const;
    void setBodyFatPercentage(double fat);

    double getChestCm() const;
    void setChestCm(double chest);

    double getWaistCm() const;
    void setWaistCm(double waist);

    double getArmsCm() const;
    void setArmsCm(double arms);

    double getThighsCm() const;
    void setThighsCm(double thighs);

    double getShouldersCm() const;
    void setShouldersCm(double shoulders);

    QString getNotes() const;
    void setNotes(const QString& notes);

    double calculateBmi() const;
    QString getBmiCategory() const;

private:
    int m_id;
    int m_memberId;
    QString m_memberName;
    QString m_recordDate;
    double m_weightKg;
    double m_heightCm;
    double m_bmi;
    double m_bodyFatPercentage;
    double m_chestCm;
    double m_waistCm;
    double m_armsCm;
    double m_thighsCm;
    double m_shouldersCm;
    QString m_notes;
};

} // namespace FitCore

#endif // PROGRESSRECORD_H
