#include "models/Exercise.h"

namespace FitCore {

Exercise::Exercise()
    : m_id(0), m_difficulty("Intermediate"), m_status("Active") {}

int Exercise::getId() const { return m_id; }
void Exercise::setId(int id) { m_id = id; }

QString Exercise::getExerciseName() const { return m_exerciseName; }
void Exercise::setExerciseName(const QString& name) { m_exerciseName = name; }

QString Exercise::getMuscleGroup() const { return m_muscleGroup; }
void Exercise::setMuscleGroup(const QString& muscleGroup) { m_muscleGroup = muscleGroup; }

QString Exercise::getEquipmentRequired() const { return m_equipmentRequired; }
void Exercise::setEquipmentRequired(const QString& equipment) { m_equipmentRequired = equipment; }

QString Exercise::getDifficulty() const { return m_difficulty; }
void Exercise::setDifficulty(const QString& difficulty) { m_difficulty = difficulty; }

QString Exercise::getInstructions() const { return m_instructions; }
void Exercise::setInstructions(const QString& instructions) { m_instructions = instructions; }

QString Exercise::getVideoUrl() const { return m_videoUrl; }
void Exercise::setVideoUrl(const QString& url) { m_videoUrl = url; }

QString Exercise::getStatus() const { return m_status; }
void Exercise::setStatus(const QString& status) { m_status = status; }

} // namespace FitCore
