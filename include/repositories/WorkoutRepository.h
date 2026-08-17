#ifndef WORKOUTREPOSITORY_H
#define WORKOUTREPOSITORY_H

#include "models/Exercise.h"
#include "models/WorkoutPlan.h"
#include <optional>
#include <vector>

namespace FitCore {

struct WorkoutLogItem {
    int logId{0};
    int memberId{0};
    QString memberName;
    int workoutPlanId{0};
    int exerciseId{0};
    QString exerciseName;
    QString logDate;
    int setsCompleted{0};
    int repsCompleted{0};
    double weightUsedKg{0.0};
    int durationMinutes{0};
    QString notes;
};

class WorkoutRepository {
public:
    WorkoutRepository() = default;

    // Exercise Library CRUD
    std::optional<Exercise> findExerciseById(int id);
    std::vector<Exercise> findAllExercises(const QString& muscleGroupFilter = "");
    bool createExercise(Exercise& exercise);
    bool updateExercise(const Exercise& exercise);

    // Workout Plans CRUD
    std::optional<WorkoutPlan> findPlanById(int id);
    std::vector<WorkoutPlan> findPlansByMemberId(int memberId);
    bool createWorkoutPlan(WorkoutPlan& plan);
    bool updateWorkoutPlan(const WorkoutPlan& plan);

    // Workout Logs
    bool logWorkout(WorkoutLogItem& log);
    std::vector<WorkoutLogItem> getWorkoutLogsByMember(int memberId);
};

} // namespace FitCore

#endif // WORKOUTREPOSITORY_H
