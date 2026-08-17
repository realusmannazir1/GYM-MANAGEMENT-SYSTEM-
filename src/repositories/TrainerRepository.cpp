#include "repositories/TrainerRepository.h"
#include "database/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

namespace FitCore {

static Trainer mapTrainer(const QSqlQuery& query) {
    Trainer t;
    t.setId(query.value("trainer_id").toInt());
    t.setEmployeeNumber(query.value("employee_number").toString());
    t.setFirstName(query.value("first_name").toString());
    t.setLastName(query.value("last_name").toString());
    t.setPhone(query.value("phone").toString());
    t.setEmail(query.value("email").toString());
    t.setSpecialization(query.value("specialization").toString());
    t.setExperienceYears(query.value("experience_years").toInt());
    t.setSalary(query.value("salary").toDouble());
    t.setJoiningDate(query.value("joining_date").toString());
    t.setStatus(query.value("status").toString());
    return t;
}

std::optional<Trainer> TrainerRepository::findById(int id) {
    QSqlQuery query = DatabaseManager::instance().prepare("SELECT * FROM trainers WHERE trainer_id = ?;");
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        return mapTrainer(query);
    }
    return std::nullopt;
}

std::vector<Trainer> TrainerRepository::findAll(bool activeOnly) {
    std::vector<Trainer> list;
    QString sql = "SELECT * FROM trainers";
    if (activeOnly) {
        sql += " WHERE status = 'Active'";
    }
    sql += " ORDER BY trainer_id ASC;";

    QSqlQuery query(sql, DatabaseManager::instance().getDatabase());
    while (query.next()) {
        list.push_back(mapTrainer(query));
    }
    return list;
}

bool TrainerRepository::create(Trainer& trainer) {
    if (trainer.getEmployeeNumber().isEmpty()) {
        trainer.setEmployeeNumber(generateNextEmployeeNumber());
    }

    QSqlQuery query = DatabaseManager::instance().prepare(
        "INSERT INTO trainers (employee_number, first_name, last_name, phone, email, specialization, experience_years, salary, joining_date, status) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"
    );
    query.addBindValue(trainer.getEmployeeNumber());
    query.addBindValue(trainer.getFirstName());
    query.addBindValue(trainer.getLastName());
    query.addBindValue(trainer.getPhone());
    query.addBindValue(trainer.getEmail());
    query.addBindValue(trainer.getSpecialization());
    query.addBindValue(trainer.getExperienceYears());
    query.addBindValue(trainer.getSalary());
    query.addBindValue(trainer.getJoiningDate());
    query.addBindValue(trainer.getStatus());

    if (query.exec()) {
        trainer.setId(query.lastInsertId().toInt());
        return true;
    }
    return false;
}

bool TrainerRepository::update(const Trainer& trainer) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "UPDATE trainers SET first_name = ?, last_name = ?, phone = ?, email = ?, specialization = ?, "
        "experience_years = ?, salary = ?, status = ? WHERE trainer_id = ?;"
    );
    query.addBindValue(trainer.getFirstName());
    query.addBindValue(trainer.getLastName());
    query.addBindValue(trainer.getPhone());
    query.addBindValue(trainer.getEmail());
    query.addBindValue(trainer.getSpecialization());
    query.addBindValue(trainer.getExperienceYears());
    query.addBindValue(trainer.getSalary());
    query.addBindValue(trainer.getStatus());
    query.addBindValue(trainer.getId());
    return query.exec();
}

bool TrainerRepository::remove(int id) {
    QSqlQuery query = DatabaseManager::instance().prepare("UPDATE trainers SET status = 'Inactive' WHERE trainer_id = ?;");
    query.addBindValue(id);
    return query.exec();
}

QString TrainerRepository::generateNextEmployeeNumber() {
    QSqlQuery query("SELECT MAX(trainer_id) FROM trainers;", DatabaseManager::instance().getDatabase());
    int maxId = 0;
    if (query.next()) maxId = query.value(0).toInt();
    return QString("TRN-%1").arg(maxId + 1, 6, 10, QChar('0'));
}

bool TrainerRepository::addSpecialization(int trainerId, const QString& spec) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "INSERT INTO trainer_specializations (trainer_id, specialization_name) VALUES (?, ?);"
    );
    query.addBindValue(trainerId);
    query.addBindValue(spec);
    return query.exec();
}

std::vector<QString> TrainerRepository::getSpecializations(int trainerId) {
    std::vector<QString> list;
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT specialization_name FROM trainer_specializations WHERE trainer_id = ?;"
    );
    query.addBindValue(trainerId);
    if (query.exec()) {
        while (query.next()) list.push_back(query.value(0).toString());
    }
    return list;
}

bool TrainerRepository::assignMemberToTrainer(int trainerId, int memberId) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "INSERT INTO member_trainer_assignments (trainer_id, member_id, start_date, status) "
        "VALUES (?, ?, DATE('now'), 'Active');"
    );
    query.addBindValue(trainerId);
    query.addBindValue(memberId);
    return query.exec();
}

std::vector<int> TrainerRepository::getAssignedMemberIds(int trainerId) {
    std::vector<int> list;
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT member_id FROM member_trainer_assignments WHERE trainer_id = ? AND status = 'Active';"
    );
    query.addBindValue(trainerId);
    if (query.exec()) {
        while (query.next()) list.push_back(query.value(0).toInt());
    }
    return list;
}

bool TrainerRepository::hasSchedulingConflict(int trainerId, const QString& date, const QString& startTime, const QString& endTime, int excludeSessionId) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT COUNT(*) FROM training_sessions "
        "WHERE trainer_id = ? AND session_date = ? AND status != 'Cancelled' "
        "AND session_id != ? AND ((start_time < ? AND end_time > ?));"
    );
    query.addBindValue(trainerId);
    query.addBindValue(date);
    query.addBindValue(excludeSessionId);
    query.addBindValue(endTime);
    query.addBindValue(startTime);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}

bool TrainerRepository::createSession(PersonalTrainingSession& session) {
    if (hasSchedulingConflict(session.trainerId, session.sessionDate, session.startTime, session.endTime)) {
        return false;
    }

    QSqlQuery query = DatabaseManager::instance().prepare(
        "INSERT INTO training_sessions (trainer_id, member_id, session_date, start_time, end_time, session_type, notes, status) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?);"
    );
    query.addBindValue(session.trainerId);
    query.addBindValue(session.memberId);
    query.addBindValue(session.sessionDate);
    query.addBindValue(session.startTime);
    query.addBindValue(session.endTime);
    query.addBindValue(session.sessionType);
    query.addBindValue(session.notes);
    query.addBindValue(session.status);

    if (query.exec()) {
        session.sessionId = query.lastInsertId().toInt();
        return true;
    }
    return false;
}

std::vector<PersonalTrainingSession> TrainerRepository::getSessionsForTrainer(int trainerId, const QString& date) {
    std::vector<PersonalTrainingSession> list;
    QString sql = "SELECT ts.*, (t.first_name || ' ' || t.last_name) AS trainer_name, "
                  "(m.first_name || ' ' || m.last_name) AS member_name "
                  "FROM training_sessions ts "
                  "JOIN trainers t ON ts.trainer_id = t.trainer_id "
                  "JOIN members m ON ts.member_id = m.member_id "
                  "WHERE ts.trainer_id = ? ";
    if (!date.isEmpty()) {
        sql += "AND ts.session_date = '" + date + "' ";
    }
    sql += "ORDER BY ts.session_date ASC, ts.start_time ASC;";

    QSqlQuery query = DatabaseManager::instance().prepare(sql);
    query.addBindValue(trainerId);

    if (query.exec()) {
        while (query.next()) {
            PersonalTrainingSession s;
            s.sessionId = query.value("session_id").toInt();
            s.trainerId = query.value("trainer_id").toInt();
            s.trainerName = query.value("trainer_name").toString();
            s.memberId = query.value("member_id").toInt();
            s.memberName = query.value("member_name").toString();
            s.sessionDate = query.value("session_date").toString();
            s.startTime = query.value("start_time").toString();
            s.endTime = query.value("end_time").toString();
            s.sessionType = query.value("session_type").toString();
            s.notes = query.value("notes").toString();
            s.status = query.value("status").toString();
            list.push_back(s);
        }
    }
    return list;
}

} // namespace FitCore
