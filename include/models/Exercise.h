#ifndef EXERCISE_H
#define EXERCISE_H

#include <QString>

namespace FitCore {

class Exercise {
public:
    Exercise();

    int getId() const;
    void setId(int id);

    QString getExerciseName() const;
    void setExerciseName(const QString& name);

    QString getMuscleGroup() const;
    void setMuscleGroup(const QString& muscleGroup);

    QString getEquipmentRequired() const;
    void setEquipmentRequired(const QString& equipment);

    QString getDifficulty() const;
    void setDifficulty(const QString& difficulty);

    QString getInstructions() const;
    void setInstructions(const QString& instructions);

    QString getVideoUrl() const;
    void setVideoUrl(const QString& url);

    QString getStatus() const;
    void setStatus(const QString& status);

private:
    int m_id;
    QString m_exerciseName;
    QString m_muscleGroup;
    QString m_equipmentRequired;
    QString m_difficulty;
    QString m_instructions;
    QString m_videoUrl;
    QString m_status;
};

} // namespace FitCore

#endif // EXERCISE_H
