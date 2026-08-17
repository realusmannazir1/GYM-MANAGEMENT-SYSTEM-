#ifndef TRAINERSWIDGET_H
#define TRAINERSWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include "repositories/TrainerRepository.h"
#include "repositories/MemberRepository.h"

namespace FitCore {

class TrainersWidget : public QWidget {
    Q_OBJECT
public:
    explicit TrainersWidget(QWidget *parent = nullptr);

    void refreshData();

private slots:
    void onAddTrainerClicked();
    void onAssignMemberClicked(int trainerId);
    void onScheduleSessionClicked(int trainerId);

private:
    void setupUi();
    void loadTrainersTable();
    void loadSessionsTable();

    QTableWidget *m_trainersTable;
    QTableWidget *m_sessionsTable;

    TrainerRepository m_trainerRepo;
    MemberRepository m_memberRepo;
};

} // namespace FitCore

#endif // TRAINERSWIDGET_H
