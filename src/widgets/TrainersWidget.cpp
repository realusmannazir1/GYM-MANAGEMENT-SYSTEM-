#include "widgets/TrainersWidget.h"
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
#include <QDateEdit>
#include <QTimeEdit>
#include <QLabel>

namespace FitCore {

TrainersWidget::TrainersWidget(QWidget *parent)
    : QWidget(parent) {
    setupUi();
    refreshData();
}

void TrainersWidget::setupUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    QHBoxLayout *topLayout = new QHBoxLayout();
    QLabel *title = new QLabel("Trainer & Personal Training Management", this);
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #F8FAFC;");

    QPushButton *addTrainerBtn = new QPushButton("+ Add New Trainer", this);
    addTrainerBtn->setObjectName("successBtn");

    topLayout->addWidget(title);
    topLayout->addStretch();
    topLayout->addWidget(addTrainerBtn);
    mainLayout->addLayout(topLayout);

    QTabWidget *tabs = new QTabWidget(this);

    // Tab 1: Trainers Directory
    QWidget *tTab = new QWidget();
    QVBoxLayout *tLayout = new QVBoxLayout(tTab);
    m_trainersTable = new QTableWidget(tTab);
    m_trainersTable->setColumnCount(7);
    m_trainersTable->setHorizontalHeaderLabels({"Employee #", "Full Name", "Specialization", "Experience", "Phone", "Status", "Actions"});
    m_trainersTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_trainersTable->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    tLayout->addWidget(m_trainersTable);
    tabs->addTab(tTab, "Trainers Directory");

    // Tab 2: Scheduled Training Sessions
    QWidget *sTab = new QWidget();
    QVBoxLayout *sLayout = new QVBoxLayout(sTab);
    m_sessionsTable = new QTableWidget(sTab);
    m_sessionsTable->setColumnCount(6);
    m_sessionsTable->setHorizontalHeaderLabels({"Trainer", "Member", "Date", "Start Time", "End Time", "Status"});
    m_sessionsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    sLayout->addWidget(m_sessionsTable);
    tabs->addTab(sTab, "Training Sessions Calendar");

    mainLayout->addWidget(tabs);

    connect(addTrainerBtn, &QPushButton::clicked, this, &TrainersWidget::onAddTrainerClicked);
}

void TrainersWidget::refreshData() {
    loadTrainersTable();
    loadSessionsTable();
}

void TrainersWidget::loadTrainersTable() {
    auto list = m_trainerRepo.findAll();
    m_trainersTable->setRowCount(0);
    int r = 0;

    for (const auto& t : list) {
        m_trainersTable->insertRow(r);
        m_trainersTable->setItem(r, 0, new QTableWidgetItem(t.getEmployeeNumber()));
        m_trainersTable->setItem(r, 1, new QTableWidgetItem(t.getFullName()));
        m_trainersTable->setItem(r, 2, new QTableWidgetItem(t.getSpecialization()));
        m_trainersTable->setItem(r, 3, new QTableWidgetItem(QString("%1 yrs").arg(t.getExperienceYears())));
        m_trainersTable->setItem(r, 4, new QTableWidgetItem(t.getPhone()));

        QLabel *badge = new QLabel(t.getStatus(), this);
        badge->setStyleSheet(ThemeManager::getBadgeStyle(t.getStatus()));
        badge->setAlignment(Qt::AlignCenter);
        m_trainersTable->setCellWidget(r, 5, badge);

        QWidget *actWidget = new QWidget(this);
        QHBoxLayout *actLayout = new QHBoxLayout(actWidget);
        actLayout->setContentsMargins(2, 2, 2, 2);
        actLayout->setSpacing(4);

        QPushButton *assignBtn = new QPushButton("Assign", actWidget);
        assignBtn->setObjectName("secondaryBtn");
        assignBtn->setStyleSheet("padding: 4px 8px; font-size: 11px;");

        QPushButton *schedBtn = new QPushButton("Schedule", actWidget);
        schedBtn->setStyleSheet("padding: 4px 8px; font-size: 11px;");

        int id = t.getId();
        connect(assignBtn, &QPushButton::clicked, this, [this, id]() { onAssignMemberClicked(id); });
        connect(schedBtn, &QPushButton::clicked, this, [this, id]() { onScheduleSessionClicked(id); });

        actLayout->addWidget(assignBtn);
        actLayout->addWidget(schedBtn);
        m_trainersTable->setCellWidget(r, 6, actWidget);
        r++;
    }
}

void TrainersWidget::loadSessionsTable() {
    auto trainers = m_trainerRepo.findAll();
    m_sessionsTable->setRowCount(0);
    int r = 0;

    for (const auto& tr : trainers) {
        auto sessions = m_trainerRepo.getSessionsForTrainer(tr.getId());
        for (const auto& s : sessions) {
            m_sessionsTable->insertRow(r);
            m_sessionsTable->setItem(r, 0, new QTableWidgetItem(s.trainerName));
            m_sessionsTable->setItem(r, 1, new QTableWidgetItem(s.memberName));
            m_sessionsTable->setItem(r, 2, new QTableWidgetItem(s.sessionDate));
            m_sessionsTable->setItem(r, 3, new QTableWidgetItem(s.startTime));
            m_sessionsTable->setItem(r, 4, new QTableWidgetItem(s.endTime));
            m_sessionsTable->setItem(r, 5, new QTableWidgetItem(s.status));
            r++;
        }
    }
}

void TrainersWidget::onAddTrainerClicked() {
    QDialog dlg(this);
    dlg.setWindowTitle("Register New Trainer");
    dlg.setFixedSize(480, 480);
    dlg.setStyleSheet(ThemeManager::getDarkThemeQss());

    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    QFormLayout *form = new QFormLayout();

    QLineEdit *codeInput = new QLineEdit(&dlg);
    codeInput->setText(m_trainerRepo.generateNextEmployeeNumber());
    codeInput->setReadOnly(true);

    QLineEdit *fnInput = new QLineEdit(&dlg);
    QLineEdit *lnInput = new QLineEdit(&dlg);
    QLineEdit *phoneInput = new QLineEdit(&dlg);
    QLineEdit *emailInput = new QLineEdit(&dlg);
    QLineEdit *specInput = new QLineEdit(&dlg);
    specInput->setPlaceholderText("e.g. Bodybuilding & Strength");

    QSpinBox *expSpin = new QSpinBox(&dlg);
    expSpin->setRange(0, 50);

    QDoubleSpinBox *salarySpin = new QDoubleSpinBox(&dlg);
    salarySpin->setRange(0.0, 500000.0);
    salarySpin->setPrefix("Rs. ");

    form->addRow("Employee #:", codeInput);
    form->addRow("First Name *:", fnInput);
    form->addRow("Last Name *:", lnInput);
    form->addRow("Phone *:", phoneInput);
    form->addRow("Email:", emailInput);
    form->addRow("Specialization:", specInput);
    form->addRow("Experience (Years):", expSpin);
    form->addRow("Salary (PKR):", salarySpin);

    layout->addLayout(form);

    QHBoxLayout *btnBox = new QHBoxLayout();
    QPushButton *cancelBtn = new QPushButton("Cancel", &dlg);
    cancelBtn->setObjectName("secondaryBtn");
    QPushButton *saveBtn = new QPushButton("Register Trainer", &dlg);
    saveBtn->setObjectName("successBtn");

    btnBox->addWidget(cancelBtn);
    btnBox->addWidget(saveBtn);
    layout->addLayout(btnBox);

    connect(cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, [&]() {
        if (fnInput->text().trimmed().isEmpty() || phoneInput->text().trimmed().isEmpty()) {
            QMessageBox::warning(&dlg, "Validation Error", "First Name and Phone are required.");
            return;
        }

        Trainer t;
        t.setEmployeeNumber(codeInput->text());
        t.setFirstName(fnInput->text().trimmed());
        t.setLastName(lnInput->text().trimmed());
        t.setPhone(phoneInput->text().trimmed());
        t.setEmail(emailInput->text().trimmed());
        t.setSpecialization(specInput->text().trimmed());
        t.setExperienceYears(expSpin->value());
        t.setSalary(salarySpin->value());
        t.setJoiningDate(QDate::currentDate().toString("yyyy-MM-dd"));
        t.setStatus("Active");

        if (m_trainerRepo.create(t)) {
            dlg.accept();
            refreshData();
        } else {
            QMessageBox::critical(&dlg, "Database Error", "Failed to create trainer.");
        }
    });

    dlg.exec();
}

void TrainersWidget::onAssignMemberClicked(int trainerId) {
    QDialog dlg(this);
    dlg.setWindowTitle("Assign Member to Trainer");
    dlg.setFixedSize(420, 220);
    dlg.setStyleSheet(ThemeManager::getDarkThemeQss());

    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    QFormLayout *form = new QFormLayout();

    QComboBox *memberCombo = new QComboBox(&dlg);
    auto members = m_memberRepo.findAll();
    for (const auto& m : members) {
        memberCombo->addItem(m.getFullName() + " (" + m.getMembershipNumber() + ")", m.getId());
    }

    form->addRow("Select Member:", memberCombo);
    layout->addLayout(form);

    QHBoxLayout *btnBox = new QHBoxLayout();
    QPushButton *cancelBtn = new QPushButton("Cancel", &dlg);
    cancelBtn->setObjectName("secondaryBtn");
    QPushButton *assignBtn = new QPushButton("Assign Member", &dlg);
    assignBtn->setObjectName("successBtn");

    btnBox->addWidget(cancelBtn);
    btnBox->addWidget(assignBtn);
    layout->addLayout(btnBox);

    connect(cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(assignBtn, &QPushButton::clicked, [&]() {
        if (memberCombo->currentIndex() >= 0) {
            int memberId = memberCombo->currentData().toInt();
            if (m_trainerRepo.assignMemberToTrainer(trainerId, memberId)) {
                QMessageBox::information(&dlg, "Assignment Successful", "Member assigned to trainer.");
                dlg.accept();
            } else {
                QMessageBox::critical(&dlg, "Error", "Failed to assign member.");
            }
        }
    });

    dlg.exec();
}

void TrainersWidget::onScheduleSessionClicked(int trainerId) {
    QDialog dlg(this);
    dlg.setWindowTitle("Schedule Personal Training Session");
    dlg.setFixedSize(460, 380);
    dlg.setStyleSheet(ThemeManager::getDarkThemeQss());

    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    QFormLayout *form = new QFormLayout();

    QComboBox *memberCombo = new QComboBox(&dlg);
    auto members = m_memberRepo.findAll();
    for (const auto& m : members) {
        memberCombo->addItem(m.getFullName() + " (" + m.getMembershipNumber() + ")", m.getId());
    }

    QDateEdit *dateEdit = new QDateEdit(&dlg);
    dateEdit->setDisplayFormat("yyyy-MM-dd");
    dateEdit->setDate(QDate::currentDate());
    dateEdit->setCalendarPopup(true);

    QTimeEdit *startTimeEdit = new QTimeEdit(&dlg);
    startTimeEdit->setDisplayFormat("hh:mm");
    startTimeEdit->setTime(QTime(10, 0));

    QTimeEdit *endTimeEdit = new QTimeEdit(&dlg);
    endTimeEdit->setDisplayFormat("hh:mm");
    endTimeEdit->setTime(QTime(11, 0));

    QLineEdit *notesInput = new QLineEdit(&dlg);

    form->addRow("Select Member *:", memberCombo);
    form->addRow("Session Date *:", dateEdit);
    form->addRow("Start Time *:", startTimeEdit);
    form->addRow("End Time *:", endTimeEdit);
    form->addRow("Session Notes:", notesInput);

    layout->addLayout(form);

    QHBoxLayout *btnBox = new QHBoxLayout();
    QPushButton *cancelBtn = new QPushButton("Cancel", &dlg);
    cancelBtn->setObjectName("secondaryBtn");
    QPushButton *saveBtn = new QPushButton("Schedule Session", &dlg);
    saveBtn->setObjectName("successBtn");

    btnBox->addWidget(cancelBtn);
    btnBox->addWidget(saveBtn);
    layout->addLayout(btnBox);

    connect(cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, [&]() {
        if (memberCombo->currentIndex() < 0) return;

        PersonalTrainingSession s;
        s.trainerId = trainerId;
        s.memberId = memberCombo->currentData().toInt();
        s.sessionDate = dateEdit->date().toString("yyyy-MM-dd");
        s.startTime = startTimeEdit->time().toString("hh:mm:ss");
        s.endTime = endTimeEdit->time().toString("hh:mm:ss");
        s.notes = notesInput->text().trimmed();
        s.status = "Scheduled";

        if (m_trainerRepo.createSession(s)) {
            QMessageBox::information(&dlg, "Session Scheduled", "Personal training session scheduled cleanly.");
            dlg.accept();
            refreshData();
        } else {
            QMessageBox::warning(&dlg, "Scheduling Conflict", "Trainer already has an overlapping session during this time slot.");
        }
    });

    dlg.exec();
}

} // namespace FitCore
