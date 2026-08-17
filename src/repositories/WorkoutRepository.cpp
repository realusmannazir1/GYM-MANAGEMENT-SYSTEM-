#include "repositories/WorkoutRepository.h"
#include "database/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDate>

namespace FitCore {

static Exercise mapExercise(const QSqlQuery& query) {
    Exercise e;
    e.setId(query.value("exercise_id").toInt());
    e.setExerciseName(query.value("exercise_name").toString());
    e.setMuscleGroup(query.value("muscle_group").toString());
    e.setEquipmentRequired(query.value("equipment_required").toString());
    e.setDifficulty(query.value("difficulty").toString());
    e.setInstructions(query.value("instructions").toString());
    e.setVideoUrl(query.value("video_url").toString());
    e.setStatus(query.value("status").toString());
    return e;
}

std::optional<Exercise> WorkoutRepository::findExerciseById(int id) {
    QSqlQuery query = DatabaseManager::instance().prepare("SELECT * FROM exercises WHERE exercise_id = ?;");
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        return mapExercise(query);
    }
    return std::nullopt;
}

std::vector<Exercise> WorkoutRepository::findAllExercises(const QString& muscleGroupFilter) {
    std::vector<Exercise> list;
    QString sql = "SELECT * FROM exercises ";
    if (!muscleGroupFilter.isEmpty() && muscleGroupFilter != "All") {
        sql += "WHERE muscle_group = '" + muscleGroupFilter + "' ";
    }
    sql += "ORDER BY exercise_name ASC;";

    QSqlQuery query(sql, DatabaseManager::instance().getDatabase());
    while (query.next()) {
        list.push_back(mapExercise(query));
    }
    return list;
}

bool WorkoutRepository::createExercise(Exercise& exercise) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "INSERT INTO exercises (exercise_name, muscle_group, equipment_required, difficulty, instructions, video_url, status) "
        "VALUES (?, ?, ?, ?, ?, ?, ?);"
    );
    query.addBindValue(exercise.getExerciseName());
    query.addBindValue(exercise.getMuscleGroup());
    query.addBindValue(exercise.getEquipmentRequired());
    query.addBindValue(exercise.getDifficulty());
    query.addBindValue(exercise.getInstructions());
    query.addBindValue(exercise.getVideoUrl());
    query.addBindValue(exercise.getStatus());

    if (query.exec()) {
        exercise.setId(query.lastInsertId().toInt());
        return true;
    }
    return false;
}

bool WorkoutRepository::updateExercise(const Exercise& exercise) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "UPDATE exercises SET exercise_name = ?, muscle_group = ?, equipment_required = ?, "
        "difficulty = ?, instructions = ?, video_url = ?, status = ? WHERE exercise_id = ?;"
    );
    query.addBindValue(exercise.getExerciseName());
    query.addBindValue(exercise.getMuscleGroup());
    query.addBindValue(exercise.getEquipmentRequired());
    query.addBindValue(exercise.getDifficulty());
    query.addBindValue(exercise.getInstructions());
    query.addBindValue(exercise.getVideoUrl());
    query.addBindValue(exercise.getStatus());
    query.addBindValue(exercise.getId());
    return query.exec();
}

std::optional<WorkoutPlan> WorkoutRepository::findPlanById(int id) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT wp.*, (m.first_name || ' ' || m.last_name) AS member_name, "
        "(t.first_name || ' ' || t.last_name) AS trainer_name "
        "FROM workout_plans wp "
        "JOIN members m ON wp.member_id = m.member_id "
        "LEFT JOIN trainers t ON wp.trainer_id = t.trainer_id "
        "WHERE wp.workout_plan_id = ?;"
    );
    query.addBindValue(id);

    if (query.exec() && query.next()) {
        WorkoutPlan plan;
        plan.setId(query.value("workout_plan_id").toInt());
        plan.setMemberId(query.value("member_id").toInt());
        plan.setMemberName(query.value("member_name").toString());
        plan.setTrainerId(query.value("trainer_id").toInt());
        plan.setTrainerName(query.value("trainer_name").toString());
        plan.setPlanName(query.value("plan_name").toString());
        plan.setGoal(query.value("goal").toString());
        plan.setStartDate(query.value("start_date").toString());
        plan.setEndDate(query.value("end_date").toString());
        plan.setNotes(query.value("notes").toString());
        plan.setStatus(query.value("status").toString());

        // Fetch exercise items
        QSqlQuery itemQuery = DatabaseManager::instance().prepare(
            "SELECT wpe.*, e.exercise_name, e.muscle_group "
            "FROM workout_plan_exercises wpe "
            "JOIN exercises e ON wpe.exercise_id = e.exercise_id "
            "WHERE wpe.workout_plan_id = ? ORDER BY wpe.exercise_order ASC;"
        );
        itemQuery.addBindValue(id);
        if (itemQuery.exec()) {
            while (itemQuery.next()) {
                WorkoutPlanExerciseItem item;
                item.id = itemQuery.value("id").toInt();
                item.workoutPlanId = id;
                item.exerciseId = itemQuery.value("exercise_id").toInt();
                item.exerciseName = itemQuery.value("exercise_name").toString();
                item.muscleGroup = itemQuery.value("muscle_group").toString();
                item.sets = itemQuery.value("sets").toInt();
                item.repetitions = itemQuery.value("repetitions").toInt();
                item.weightKg = itemQuery.value("weight_kg").toDouble();
                item.restTimeSeconds = itemQuery.value("rest_time_seconds").toInt();
                item.exerciseOrder = itemQuery.value("exercise_order").toInt();
                item.notes = itemQuery.value("notes").toString();
                plan.addExercise(item);
            }
        }
        return plan;
    }
    return std::nullopt;
}

std::vector<WorkoutPlan> WorkoutRepository::findPlansByMemberId(int memberId) {
    std::vector<WorkoutPlan> list;
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT workout_plan_id FROM workout_plans WHERE member_id = ? ORDER BY workout_plan_id DESC;"
    );
    query.addBindValue(memberId);
    if (query.exec()) {
        while (query.next()) {
            auto planOpt = findPlanById(query.value(0).toInt());
            if (planOpt.has_value()) {
                list.push_back(planOpt.value());
            }
        }
    }
    return list;
}

bool WorkoutRepository::createWorkoutPlan(WorkoutPlan& plan) {
    DatabaseManager::instance().beginTransaction();

    QSqlQuery query = DatabaseManager::instance().prepare(
        "INSERT INTO workout_plans (member_id, trainer_id, plan_name, goal, start_date, end_date, notes, status) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?);"
    );
    query.addBindValue(plan.getMemberId());
    query.addBindValue(plan.getTrainerId() > 0 ? QVariant(plan.getTrainerId()) : QVariant(QVariant::Int));
    query.addBindValue(plan.getPlanName());
    query.addBindValue(plan.getGoal());
    query.addBindValue(plan.getStartDate().isEmpty() ? QDate::currentDate().toString("yyyy-MM-dd") : plan.getStartDate());
    query.addBindValue(plan.getEndDate());
    query.addBindValue(plan.getNotes());
    query.addBindValue(plan.getStatus());

    if (!query.exec()) {
        DatabaseManager::instance().rollback();
        return false;
    }

    int planId = query.lastInsertId().toInt();
    plan.setId(planId);

    int order = 1;
    for (const auto& item : plan.getExercises()) {
        QSqlQuery itemQuery = DatabaseManager::instance().prepare(
            "INSERT INTO workout_plan_exercises (workout_plan_id, exercise_id, sets, repetitions, weight_kg, rest_time_seconds, exercise_order, notes) "
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?);"
        );
        itemQuery.addBindValue(planId);
        itemQuery.addBindValue(item.exerciseId);
        itemQuery.addBindValue(item.sets);
        itemQuery.addBindValue(item.repetitions);
        itemQuery.addBindValue(item.weightKg);
        itemQuery.addBindValue(item.restTimeSeconds);
        itemQuery.addBindValue(order++);
        itemQuery.addBindValue(item.notes);

        if (!itemQuery.exec()) {
            DatabaseManager::instance().rollback();
            return false;
        }
    }

    return DatabaseManager::instance().commit();
}

bool WorkoutRepository::updateWorkoutPlan(const WorkoutPlan& plan) {
    DatabaseManager::instance().beginTransaction();

    QSqlQuery query = DatabaseManager::instance().prepare(
        "UPDATE workout_plans SET plan_name = ?, goal = ?, start_date = ?, end_date = ?, notes = ?, status = ? "
        "WHERE workout_plan_id = ?;"
    );
    query.addBindValue(plan.getPlanName());
    query.addBindValue(plan.getGoal());
    query.addBindValue(plan.getStartDate());
    query.addBindValue(plan.getEndDate());
    query.addBindValue(plan.getNotes());
    query.addBindValue(plan.getStatus());
    query.addBindValue(plan.getId());

    if (!query.exec()) {
        DatabaseManager::instance().rollback();
        return false;
    }

    // Delete existing exercises and re-insert updated list
    QSqlQuery delQuery = DatabaseManager::instance().prepare("DELETE FROM workout_plan_exercises WHERE workout_plan_id = ?;");
    delQuery.addBindValue(plan.getId());
    if (!delQuery.exec()) {
        DatabaseManager::instance().rollback();
        return false;
    }

    int order = 1;
    for (const auto& item : plan.getExercises()) {
        QSqlQuery itemQuery = DatabaseManager::instance().prepare(
            "INSERT INTO workout_plan_exercises (workout_plan_id, exercise_id, sets, repetitions, weight_kg, rest_time_seconds, exercise_order, notes) "
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?);"
        );
        itemQuery.addBindValue(plan.getId());
        itemQuery.addBindValue(item.exerciseId);
        itemQuery.addBindValue(item.sets);
        itemQuery.addBindValue(item.repetitions);
        itemQuery.addBindValue(item.weightKg);
        itemQuery.addBindValue(item.restTimeSeconds);
        itemQuery.addBindValue(order++);
        itemQuery.addBindValue(item.notes);

        if (!itemQuery.exec()) {
            DatabaseManager::instance().rollback();
            return false;
        }
    }

    return DatabaseManager::instance().commit();
}

bool WorkoutRepository::logWorkout(WorkoutLogItem& log) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "INSERT INTO workout_logs (member_id, workout_plan_id, exercise_id, log_date, sets_completed, reps_completed, weight_used_kg, duration_minutes, notes) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);"
    );
    query.addBindValue(log.memberId);
    query.addBindValue(log.workoutPlanId > 0 ? QVariant(log.workoutPlanId) : QVariant(QVariant::Int));
    query.addBindValue(log.exerciseId);
    query.addBindValue(log.logDate.isEmpty() ? QDate::currentDate().toString("yyyy-MM-dd") : log.logDate);
    query.addBindValue(log.setsCompleted);
    query.addBindValue(log.repsCompleted);
    query.addBindValue(log.weightUsedKg);
    query.addBindValue(log.durationMinutes);
    query.addBindValue(log.notes);

    if (query.exec()) {
        log.logId = query.lastInsertId().toInt();
        return true;
    }
    return false;
}

std::vector<WorkoutLogItem> WorkoutRepository::getWorkoutLogsByMember(int memberId) {
    std::vector<WorkoutLogItem> list;
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT wl.*, e.exercise_name, (m.first_name || ' ' || m.last_name) AS member_name "
        "FROM workout_logs wl "
        "JOIN exercises e ON wl.exercise_id = e.exercise_id "
        "JOIN members m ON wl.member_id = m.member_id "
        "WHERE wl.member_id = ? ORDER BY wl.workout_log_id DESC;"
    );
    query.addBindValue(memberId);

    if (query.exec()) {
        while (query.next()) {
            WorkoutLogItem item;
            item.logId = query.value("workout_log_id").toInt();
            item.memberId = query.value("member_id").toInt();
            item.memberName = query.value("member_name").toString();
            item.workoutPlanId = query.value("workout_plan_id").toInt();
            item.exerciseId = query.value("exercise_id").toInt();
            item.exerciseName = query.value("exercise_name").toString();
            item.logDate = query.value("log_date").toString();
            item.setsCompleted = query.value("sets_completed").toInt();
            item.repsCompleted = query.value("reps_completed").toInt();
            item.weightUsedKg = query.value("weight_used_kg").toDouble();
            item.durationMinutes = query.value("duration_minutes").toInt();
            item.notes = query.value("notes").toString();
            list.push_back(item);
        }
    }
    return list;
}

} // namespace FitCore
