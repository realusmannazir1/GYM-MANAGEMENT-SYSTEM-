#ifndef WORKOUTPLAN_H
#define WORKOUTPLAN_H

#include <QString>
#include <vector>

namespace FitCore {

struct WorkoutPlanExerciseItem {
    int id{0};
    int workoutPlanId{0};
    int exerciseId{0};
    QString exerciseName;
    QString muscleGroup;
    int sets{3};
    int repetitions{10};
    double weightKg{0.0};
    int restTimeSeconds{60};
    int exerciseOrder{1};
    QString notes;
};

class WorkoutPlan {
public:
    WorkoutPlan();

    int getId() const;
    void setId(int id);

    int getMemberId() const;
    void setMemberId(int memberId);

    QString getMemberName() const;
    void setMemberName(const QString& name);

    int getTrainerId() const;
    void setTrainerId(int trainerId);

    QString getTrainerName() const;
    void setTrainerName(const QString& name);

    QString getPlanName() const;
    void setPlanName(const QString& name);

    QString getGoal() const;
    void setGoal(const QString& goal);

    QString getStartDate() const;
    void setStartDate(const QString& date);

    QString getEndDate() const;
    void setEndDate(const QString& date);

    QString getNotes() const;
    void setNotes(const QString& notes);

    QString getStatus() const;
    void setStatus(const QString& status);

    const std::vector<WorkoutPlanExerciseItem>& getExercises() const;
    void setExercises(const std::vector<WorkoutPlanExerciseItem>& exercises);
    void addExercise(const WorkoutPlanExerciseItem& item);

private:
    int m_id;
    int m_memberId;
    QString m_memberName;
    int m_trainerId;
    QString m_trainerName;
    QString m_planName;
    QString m_goal;
    QString m_startDate;
    QString m_endDate;
    QString m_notes;
    QString m_status;
    std::vector<WorkoutPlanExerciseItem> m_exercises;
};

} // namespace FitCore

#endif // WORKOUTPLAN_H
