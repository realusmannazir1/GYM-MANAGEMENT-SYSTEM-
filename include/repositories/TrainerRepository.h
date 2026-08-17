#ifndef TRAINERREPOSITORY_H
#define TRAINERREPOSITORY_H

#include "models/Trainer.h"
#include <optional>
#include <vector>

namespace FitCore {

struct PersonalTrainingSession {
    int sessionId{0};
    int trainerId{0};
    QString trainerName;
    int memberId{0};
    QString memberName;
    QString sessionDate;
    QString startTime;
    QString endTime;
    QString sessionType;
    QString notes;
    QString status;
};

class TrainerRepository {
public:
    TrainerRepository() = default;

    std::optional<Trainer> findById(int id);
    std::vector<Trainer> findAll(bool activeOnly = false);
    bool create(Trainer& trainer);
    bool update(const Trainer& trainer);
    bool remove(int id);
    QString generateNextEmployeeNumber();

    // Specializations & Assignments
    bool addSpecialization(int trainerId, const QString& spec);
    std::vector<QString> getSpecializations(int trainerId);
    bool assignMemberToTrainer(int trainerId, int memberId);
    std::vector<int> getAssignedMemberIds(int trainerId);

    // Personal Training Sessions
    bool hasSchedulingConflict(int trainerId, const QString& date, const QString& startTime, const QString& endTime, int excludeSessionId = 0);
    bool createSession(PersonalTrainingSession& session);
    std::vector<PersonalTrainingSession> getSessionsForTrainer(int trainerId, const QString& date = "");
};

} // namespace FitCore

#endif // TRAINERREPOSITORY_H
