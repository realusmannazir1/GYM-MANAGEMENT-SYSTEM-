#include "models/WorkoutPlan.h"

namespace FitCore {

WorkoutPlan::WorkoutPlan()
    : m_id(0), m_memberId(0), m_trainerId(0), m_goal("General Fitness"), m_status("Active") {}

int WorkoutPlan::getId() const { return m_id; }
void WorkoutPlan::setId(int id) { m_id = id; }

int WorkoutPlan::getMemberId() const { return m_memberId; }
void WorkoutPlan::setMemberId(int memberId) { m_memberId = memberId; }

QString WorkoutPlan::getMemberName() const { return m_memberName; }
void WorkoutPlan::setMemberName(const QString& name) { m_memberName = name; }

int WorkoutPlan::getTrainerId() const { return m_trainerId; }
void WorkoutPlan::setTrainerId(int trainerId) { m_trainerId = trainerId; }

QString WorkoutPlan::getTrainerName() const { return m_trainerName; }
void WorkoutPlan::setTrainerName(const QString& name) { m_trainerName = name; }

QString WorkoutPlan::getPlanName() const { return m_planName; }
void WorkoutPlan::setPlanName(const QString& name) { m_planName = name; }

QString WorkoutPlan::getGoal() const { return m_goal; }
void WorkoutPlan::setGoal(const QString& goal) { m_goal = goal; }

QString WorkoutPlan::getStartDate() const { return m_startDate; }
void WorkoutPlan::setStartDate(const QString& date) { m_startDate = date; }

QString WorkoutPlan::getEndDate() const { return m_endDate; }
void WorkoutPlan::setEndDate(const QString& date) { m_endDate = date; }

QString WorkoutPlan::getNotes() const { return m_notes; }
void WorkoutPlan::setNotes(const QString& notes) { m_notes = notes; }

QString WorkoutPlan::getStatus() const { return m_status; }
void WorkoutPlan::setStatus(const QString& status) { m_status = status; }

const std::vector<WorkoutPlanExerciseItem>& WorkoutPlan::getExercises() const { return m_exercises; }
void WorkoutPlan::setExercises(const std::vector<WorkoutPlanExerciseItem>& exercises) { m_exercises = exercises; }
void WorkoutPlan::addExercise(const WorkoutPlanExerciseItem& item) { m_exercises.push_back(item); }

} // namespace FitCore
