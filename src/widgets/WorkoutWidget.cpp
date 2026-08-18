#include "widgets/WorkoutWidget.h"
#include "utils/ThemeManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QTabWidget>
#include <QHeaderView>
#include <QDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>

namespace FitCore {

WorkoutWidget::WorkoutWidget(QWidget *parent)
    : QWidget(parent) {
    setupUi();
    refreshData();
}

void WorkoutWidget::setupUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    QHBoxLayout *topLayout = new QHBoxLayout();
    QLabel *title = new QLabel("Workout & Exercise Center", this);
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #0F172A;");

    QPushButton *addExBtn = new QPushButton("+ Add Exercise", this);
    addExBtn->setObjectName("secondaryBtn");

    QPushButton *createPlanBtn = new QPushButton("+ Build Workout Plan", this);
    createPlanBtn->setObjectName("successBtn");

    QPushButton *logWorkoutBtn = new QPushButton("Log Workout Session", this);

    topLayout->addWidget(title);
    topLayout->addStretch();
    topLayout->addWidget(addExBtn);
    topLayout->addWidget(createPlanBtn);
    topLayout->addWidget(logWorkoutBtn);
    mainLayout->addLayout(topLayout);

    QTabWidget *tabs = new QTabWidget(this);

    // Tab 1: Exercise Library
    QWidget *exTab = new QWidget();
    QVBoxLayout *exLayout = new QVBoxLayout(exTab);
    m_exercisesTable = new QTableWidget(exTab);
    m_exercisesTable->setColumnCount(5);
    m_exercisesTable->setHorizontalHeaderLabels({"Exercise Name", "Muscle Group", "Equipment Required", "Difficulty", "Status"});
    m_exercisesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    exLayout->addWidget(m_exercisesTable);
    tabs->addTab(exTab, "Exercise Library");

    // Tab 2: Assigned Workout Plans
    QWidget *plTab = new QWidget();
    QVBoxLayout *plLayout = new QVBoxLayout(plTab);
    m_plansTable = new QTableWidget(plTab);
    m_plansTable->setColumnCount(6);
    m_plansTable->setHorizontalHeaderLabels({"Plan Name", "Member", "Goal", "Start Date", "End Date", "Status"});
    m_plansTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    plLayout->addWidget(m_plansTable);
    tabs->addTab(plTab, "Workout Plans");

    // Tab 3: Workout Logs
    QWidget *lgTab = new QWidget();
    QVBoxLayout *lgLayout = new QVBoxLayout(lgTab);
    m_logsTable = new QTableWidget(lgTab);
    m_logsTable->setColumnCount(7);
    m_logsTable->setHorizontalHeaderLabels({"Date", "Member", "Exercise", "Sets", "Reps", "Weight (kg)", "Duration"});
    m_logsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    lgLayout->addWidget(m_logsTable);
    tabs->addTab(lgTab, "Workout Logs & Performance");

    mainLayout->addWidget(tabs);

    connect(addExBtn, &QPushButton::clicked, this, &WorkoutWidget::onAddExerciseClicked);
    connect(createPlanBtn, &QPushButton::clicked, this, &WorkoutWidget::onCreatePlanClicked);
    connect(logWorkoutBtn, &QPushButton::clicked, this, &WorkoutWidget::onLogWorkoutClicked);
}

void WorkoutWidget::refreshData() {
    loadExercisesTable();
    loadPlansTable();
    loadLogsTable();
}

void WorkoutWidget::loadExercisesTable() {
    auto list = m_workoutRepo.findAllExercises();
    m_exercisesTable->setRowCount(0);
    int r = 0;
    for (const auto& e : list) {
        m_exercisesTable->insertRow(r);
        m_exercisesTable->setItem(r, 0, new QTableWidgetItem(e.getExerciseName()));
        m_exercisesTable->setItem(r, 1, new QTableWidgetItem(e.getMuscleGroup()));
        m_exercisesTable->setItem(r, 2, new QTableWidgetItem(e.getEquipmentRequired()));
        m_exercisesTable->setItem(r, 3, new QTableWidgetItem(e.getDifficulty()));

        QLabel *badge = new QLabel(e.getStatus(), this);
        badge->setStyleSheet(ThemeManager::getBadgeStyle(e.getStatus()));
        badge->setAlignment(Qt::AlignCenter);
        m_exercisesTable->setCellWidget(r, 4, badge);
        r++;
    }
}

void WorkoutWidget::loadPlansTable() {
    auto members = m_memberRepo.findAll();
    m_plansTable->setRowCount(0);
    int r = 0;
    for (const auto& m : members) {
        auto plans = m_workoutRepo.findPlansByMemberId(m.getId());
        for (const auto& p : plans) {
            m_plansTable->insertRow(r);
            m_plansTable->setItem(r, 0, new QTableWidgetItem(p.getPlanName()));
            m_plansTable->setItem(r, 1, new QTableWidgetItem(p.getMemberName()));
            m_plansTable->setItem(r, 2, new QTableWidgetItem(p.getGoal()));
            m_plansTable->setItem(r, 3, new QTableWidgetItem(p.getStartDate()));
            m_plansTable->setItem(r, 4, new QTableWidgetItem(p.getEndDate()));

            QLabel *badge = new QLabel(p.getStatus(), this);
            badge->setStyleSheet(ThemeManager::getBadgeStyle(p.getStatus()));
            badge->setAlignment(Qt::AlignCenter);
            m_plansTable->setCellWidget(r, 5, badge);
            r++;
        }
    }
}

void WorkoutWidget::loadLogsTable() {
    auto members = m_memberRepo.findAll();
    m_logsTable->setRowCount(0);
    int r = 0;
    for (const auto& m : members) {
        auto logs = m_workoutRepo.getWorkoutLogsByMember(m.getId());
        for (const auto& l : logs) {
            m_logsTable->insertRow(r);
            m_logsTable->setItem(r, 0, new QTableWidgetItem(l.logDate));
            m_logsTable->setItem(r, 1, new QTableWidgetItem(l.memberName));
            m_logsTable->setItem(r, 2, new QTableWidgetItem(l.exerciseName));
            m_logsTable->setItem(r, 3, new QTableWidgetItem(QString::number(l.setsCompleted)));
            m_logsTable->setItem(r, 4, new QTableWidgetItem(QString::number(l.repsCompleted)));
            m_logsTable->setItem(r, 5, new QTableWidgetItem(QString::number(l.weightUsedKg, 'f', 1)));
            m_logsTable->setItem(r, 6, new QTableWidgetItem(QString("%1 mins").arg(l.durationMinutes)));
            r++;
        }
    }
}

void WorkoutWidget::onAddExerciseClicked() {
    QDialog dlg(this);
    dlg.setWindowTitle("Add New Exercise");
    dlg.setFixedSize(450, 420);
    dlg.setStyleSheet(ThemeManager::getDarkThemeQss());

    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    QFormLayout *form = new QFormLayout();

    QLineEdit *nameInput = new QLineEdit(&dlg);
    QComboBox *muscleCombo = new QComboBox(&dlg);
    muscleCombo->addItems({"Chest", "Back", "Legs", "Shoulders", "Arms", "Core", "Full Body", "Cardio"});

    QLineEdit *eqInput = new QLineEdit(&dlg);
    eqInput->setPlaceholderText("e.g. Barbell, Dumbbell, None");

    QComboBox *diffCombo = new QComboBox(&dlg);
    diffCombo->addItems({"Beginner", "Intermediate", "Advanced"});

    QLineEdit *instInput = new QLineEdit(&dlg);
    QLineEdit *urlInput = new QLineEdit(&dlg);

    form->addRow("Exercise Name *:", nameInput);
    form->addRow("Muscle Group *:", muscleCombo);
    form->addRow("Equipment:", eqInput);
    form->addRow("Difficulty:", diffCombo);
    form->addRow("Instructions:", instInput);
    form->addRow("Video URL:", urlInput);

    layout->addLayout(form);

    QHBoxLayout *btnBox = new QHBoxLayout();
    QPushButton *cancelBtn = new QPushButton("Cancel", &dlg);
    cancelBtn->setObjectName("secondaryBtn");
    QPushButton *saveBtn = new QPushButton("Save Exercise", &dlg);
    saveBtn->setObjectName("successBtn");

    btnBox->addWidget(cancelBtn);
    btnBox->addWidget(saveBtn);
    layout->addLayout(btnBox);

    connect(cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, [&]() {
        if (nameInput->text().trimmed().isEmpty()) return;

        Exercise e;
        e.setExerciseName(nameInput->text().trimmed());
        e.setMuscleGroup(muscleCombo->currentText());
        e.setEquipmentRequired(eqInput->text().trimmed());
        e.setDifficulty(diffCombo->currentText());
        e.setInstructions(instInput->text().trimmed());
        e.setVideoUrl(urlInput->text().trimmed());
        e.setStatus("Active");

        if (m_workoutRepo.createExercise(e)) {
            dlg.accept();
            refreshData();
        }
    });

    dlg.exec();
}

void WorkoutWidget::onCreatePlanClicked() {
    QDialog dlg(this);
    dlg.setWindowTitle("Build Personalized Workout Plan");
    dlg.setFixedSize(550, 480);
    dlg.setStyleSheet(ThemeManager::getDarkThemeQss());

    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    QFormLayout *form = new QFormLayout();

    QComboBox *memberCombo = new QComboBox(&dlg);
    auto members = m_memberRepo.findAll();
    for (const auto& m : members) {
        memberCombo->addItem(m.getFullName() + " (" + m.getMembershipNumber() + ")", m.getId());
    }

    QLineEdit *planNameInput = new QLineEdit(&dlg);
    planNameInput->setText("Custom Muscle Building Routine");

    QLineEdit *goalInput = new QLineEdit(&dlg);
    goalInput->setText("Hypertrophy & Strength");

    form->addRow("Select Member *:", memberCombo);
    form->addRow("Plan Name *:", planNameInput);
    form->addRow("Fitness Goal:", goalInput);

    layout->addLayout(form);

    QLabel *exHeader = new QLabel("Exercises Included:", &dlg);
    exHeader->setStyleSheet("font-weight: bold; margin-top: 10px;");
    layout->addWidget(exHeader);

    QTableWidget *exTable = new QTableWidget(&dlg);
    exTable->setColumnCount(5);
    exTable->setHorizontalHeaderLabels({"Exercise", "Sets", "Reps", "Weight (kg)", "Rest (sec)"});
    exTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    auto allExercises = m_workoutRepo.findAllExercises();
    exTable->setRowCount(0);
    for (int i = 0; i < qMin(5, static_cast<int>(allExercises.size())); ++i) {
        exTable->insertRow(i);
        exTable->setItem(i, 0, new QTableWidgetItem(allExercises[i].getExerciseName()));
        exTable->setItem(i, 1, new QTableWidgetItem("3"));
        exTable->setItem(i, 2, new QTableWidgetItem("12"));
        exTable->setItem(i, 3, new QTableWidgetItem("50"));
        exTable->setItem(i, 4, new QTableWidgetItem("60"));
    }

    layout->addWidget(exTable);

    QHBoxLayout *btnBox = new QHBoxLayout();
    QPushButton *cancelBtn = new QPushButton("Cancel", &dlg);
    cancelBtn->setObjectName("secondaryBtn");
    QPushButton *saveBtn = new QPushButton("Create Plan", &dlg);
    saveBtn->setObjectName("successBtn");

    btnBox->addWidget(cancelBtn);
    btnBox->addWidget(saveBtn);
    layout->addLayout(btnBox);

    connect(cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, [&]() {
        if (memberCombo->currentIndex() < 0 || planNameInput->text().trimmed().isEmpty()) return;

        WorkoutPlan plan;
        plan.setMemberId(memberCombo->currentData().toInt());
        plan.setPlanName(planNameInput->text().trimmed());
        plan.setGoal(goalInput->text().trimmed());
        plan.setStatus("Active");

        for (int r = 0; r < exTable->rowCount(); ++r) {
            if (r < static_cast<int>(allExercises.size())) {
                WorkoutPlanExerciseItem item;
                item.exerciseId = allExercises[r].getId();
                item.sets = exTable->item(r, 1)->text().toInt();
                item.repetitions = exTable->item(r, 2)->text().toInt();
                item.weightKg = exTable->item(r, 3)->text().toDouble();
                item.restTimeSeconds = exTable->item(r, 4)->text().toInt();
                plan.addExercise(item);
            }
        }

        if (m_workoutRepo.createWorkoutPlan(plan)) {
            QMessageBox::information(&dlg, "Plan Created", "Workout plan assigned to member successfully.");
            dlg.accept();
            refreshData();
        }
    });

    dlg.exec();
}

void WorkoutWidget::onLogWorkoutClicked() {
    QDialog dlg(this);
    dlg.setWindowTitle("Log Completed Workout");
    dlg.setFixedSize(450, 400);
    dlg.setStyleSheet(ThemeManager::getDarkThemeQss());

    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    QFormLayout *form = new QFormLayout();

    QComboBox *memberCombo = new QComboBox(&dlg);
    auto members = m_memberRepo.findAll();
    for (const auto& m : members) {
        memberCombo->addItem(m.getFullName() + " (" + m.getMembershipNumber() + ")", m.getId());
    }

    QComboBox *exCombo = new QComboBox(&dlg);
    auto exercises = m_workoutRepo.findAllExercises();
    for (const auto& e : exercises) {
        exCombo->addItem(e.getExerciseName(), e.getId());
    }

    QSpinBox *setsSpin = new QSpinBox(&dlg);
    setsSpin->setValue(3);

    QSpinBox *repsSpin = new QSpinBox(&dlg);
    repsSpin->setValue(10);

    QDoubleSpinBox *weightSpin = new QDoubleSpinBox(&dlg);
    weightSpin->setValue(40.0);
    weightSpin->setSuffix(" kg");

    QSpinBox *durSpin = new QSpinBox(&dlg);
    durSpin->setValue(45);
    durSpin->setSuffix(" mins");

    form->addRow("Select Member *:", memberCombo);
    form->addRow("Select Exercise *:", exCombo);
    form->addRow("Sets Completed:", setsSpin);
    form->addRow("Reps per Set:", repsSpin);
    form->addRow("Weight Used:", weightSpin);
    form->addRow("Workout Duration:", durSpin);

    layout->addLayout(form);

    QHBoxLayout *btnBox = new QHBoxLayout();
    QPushButton *cancelBtn = new QPushButton("Cancel", &dlg);
    cancelBtn->setObjectName("secondaryBtn");
    QPushButton *saveBtn = new QPushButton("Log Session", &dlg);
    saveBtn->setObjectName("successBtn");

    btnBox->addWidget(cancelBtn);
    btnBox->addWidget(saveBtn);
    layout->addLayout(btnBox);

    connect(cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, [&]() {
        if (memberCombo->currentIndex() < 0 || exCombo->currentIndex() < 0) return;

        WorkoutLogItem log;
        log.memberId = memberCombo->currentData().toInt();
        log.exerciseId = exCombo->currentData().toInt();
        log.setsCompleted = setsSpin->value();
        log.repsCompleted = repsSpin->value();
        log.weightUsedKg = weightSpin->value();
        log.durationMinutes = durSpin->value();

        if (m_workoutRepo.logWorkout(log)) {
            QMessageBox::information(&dlg, "Workout Logged", "Performance log saved cleanly.");
            dlg.accept();
            refreshData();
        }
    });

    dlg.exec();
}

} // namespace FitCore
