#ifndef WORKOUTWIDGET_H
#define WORKOUTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include "repositories/WorkoutRepository.h"
#include "repositories/MemberRepository.h"

namespace FitCore {

class WorkoutWidget : public QWidget {
    Q_OBJECT
public:
    explicit WorkoutWidget(QWidget *parent = nullptr);

    void refreshData();

private slots:
    void onAddExerciseClicked();
    void onCreatePlanClicked();
    void onLogWorkoutClicked();

private:
    void setupUi();
    void loadExercisesTable();
    void loadPlansTable();
    void loadLogsTable();

    QTableWidget *m_exercisesTable;
    QTableWidget *m_plansTable;
    QTableWidget *m_logsTable;

    WorkoutRepository m_workoutRepo;
    MemberRepository m_memberRepo;
};

} // namespace FitCore

#endif // WORKOUTWIDGET_H
