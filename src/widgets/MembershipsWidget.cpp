#include "widgets/MembershipsWidget.h"
#include "utils/ThemeManager.h"
#include "utils/ToastNotification.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QTabWidget>
#include <QHeaderView>
#include <QDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QLabel>

namespace FitCore {

MembershipsWidget::MembershipsWidget(QWidget *parent)
    : QWidget(parent) {
    setupUi();
    refreshData();
}

void MembershipsWidget::setupUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    // Header
    QHBoxLayout *topLayout = new QHBoxLayout();
    QLabel *title = new QLabel("Memberships & Renewals Control Center", this);
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #0F172A;");

    QPushButton *renewBtn = new QPushButton("Renew / Assign Membership", this);
    renewBtn->setObjectName("successBtn");

    QPushButton *addPlanBtn = new QPushButton("+ New Membership Plan", this);

    topLayout->addWidget(title);
    topLayout->addStretch();
    topLayout->addWidget(addPlanBtn);
    topLayout->addWidget(renewBtn);
    mainLayout->addLayout(topLayout);

    // Tabs
    QTabWidget *tabs = new QTabWidget(this);

    // Active Memberships
    QWidget *activeTab = new QWidget();
    QVBoxLayout *actLayout = new QVBoxLayout(activeTab);
    m_activeTable = new QTableWidget(activeTab);
    m_activeTable->setColumnCount(6);
    m_activeTable->setHorizontalHeaderLabels({"Member", "Plan", "Start Date", "End Date", "Price", "Status"});
    m_activeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_activeTable->verticalHeader()->setDefaultSectionSize(42);
    m_activeTable->verticalHeader()->setVisible(false);
    actLayout->addWidget(m_activeTable);
    tabs->addTab(activeTab, "Active Memberships");

    // Expiring Soon
    QWidget *expiringTab = new QWidget();
    QVBoxLayout *expLayout = new QVBoxLayout(expiringTab);
    m_expiringTable = new QTableWidget(expiringTab);
    m_expiringTable->setColumnCount(6);
    m_expiringTable->setHorizontalHeaderLabels({"Member", "Plan", "Start Date", "End Date", "Price", "Action"});
    m_expiringTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_expiringTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    m_expiringTable->verticalHeader()->setDefaultSectionSize(40);
    m_expiringTable->verticalHeader()->setVisible(false);
    expLayout->addWidget(m_expiringTable);
    tabs->addTab(expiringTab, "Expiring Within 7 Days");

    // Expired Memberships
    QWidget *expiredTab = new QWidget();
    QVBoxLayout *exLayout = new QVBoxLayout(expiredTab);
    m_expiredTable = new QTableWidget(expiredTab);
    m_expiredTable->setColumnCount(5);
    m_expiredTable->setHorizontalHeaderLabels({"Member", "Plan", "Start Date", "End Date", "Action"});
    m_expiredTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_expiredTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    m_expiredTable->verticalHeader()->setDefaultSectionSize(40);
    m_expiredTable->verticalHeader()->setVisible(false);
    exLayout->addWidget(m_expiredTable);
    tabs->addTab(expiredTab, "Expired Memberships");

    // Membership Plans Configuration
    QWidget *plansTab = new QWidget();
    QVBoxLayout *plLayout = new QVBoxLayout(plansTab);
    m_plansTable = new QTableWidget(plansTab);
    m_plansTable->setColumnCount(5);
    m_plansTable->setHorizontalHeaderLabels({"Plan ID", "Plan Name", "Duration (Days)", "Price (PKR)", "Access Type"});
    m_plansTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_plansTable->verticalHeader()->setDefaultSectionSize(42);
    m_plansTable->verticalHeader()->setVisible(false);
    plLayout->addWidget(m_plansTable);
    tabs->addTab(plansTab, "Plan Packages");

    mainLayout->addWidget(tabs);

    connect(renewBtn, &QPushButton::clicked, this, &MembershipsWidget::onRenewClicked);
    connect(addPlanBtn, &QPushButton::clicked, this, &MembershipsWidget::onAddPlanClicked);
}

void MembershipsWidget::refreshData() {
    m_membershipService.updateAutoExpiries();
    loadActiveTable();
    loadExpiringTable();
    loadExpiredTable();
    loadPlansTable();
}

void MembershipsWidget::loadActiveTable() {
    auto list = m_membershipRepo.findAll("Active");
    m_activeTable->setRowCount(0);
    int r = 0;
    for (const auto& ms : list) {
        m_activeTable->insertRow(r);
        m_activeTable->setItem(r, 0, new QTableWidgetItem(ms.getMemberName()));
        m_activeTable->setItem(r, 1, new QTableWidgetItem(ms.getPlanName()));
        m_activeTable->setItem(r, 2, new QTableWidgetItem(ms.getStartDate()));
        m_activeTable->setItem(r, 3, new QTableWidgetItem(ms.getEndDate()));
        m_activeTable->setItem(r, 4, new QTableWidgetItem(QString("Rs. %1").arg(ms.getFinalAmount(), 0, 'f', 0)));

        QLabel *badge = new QLabel(ms.getStatus(), this);
        badge->setStyleSheet(ThemeManager::getBadgeStyle(ms.getStatus()));
        badge->setAlignment(Qt::AlignCenter);
        m_activeTable->setCellWidget(r, 5, badge);
        r++;
    }
}

void MembershipsWidget::loadExpiringTable() {
    auto list = m_membershipRepo.findExpiringSoon(7);
    m_expiringTable->setRowCount(0);
    int r = 0;
    for (const auto& ms : list) {
        m_expiringTable->insertRow(r);
        m_expiringTable->setItem(r, 0, new QTableWidgetItem(ms.getMemberName()));
        m_expiringTable->setItem(r, 1, new QTableWidgetItem(ms.getPlanName()));
        m_expiringTable->setItem(r, 2, new QTableWidgetItem(ms.getStartDate()));
        m_expiringTable->setItem(r, 3, new QTableWidgetItem(ms.getEndDate()));
        m_expiringTable->setItem(r, 4, new QTableWidgetItem(QString("Rs. %1").arg(ms.getFinalAmount(), 0, 'f', 0)));

        QWidget *actWidget = new QWidget(this);
        QHBoxLayout *actLayout = new QHBoxLayout(actWidget);
        actLayout->setContentsMargins(2, 2, 2, 2);
        actLayout->setAlignment(Qt::AlignCenter);

        QPushButton *renewBtn = new QPushButton("🔄", actWidget);
        renewBtn->setObjectName("iconSuccessBtn");
        renewBtn->setToolTip("Renew Membership Plan");
        renewBtn->setCursor(Qt::PointingHandCursor);
        renewBtn->setFixedSize(30, 28);
        connect(renewBtn, &QPushButton::clicked, this, &MembershipsWidget::onRenewClicked);
        m_expiringTable->setCellWidget(r, 5, actWidget);
        r++;
    }
}

void MembershipsWidget::loadExpiredTable() {
    auto list = m_membershipRepo.findExpired();
    m_expiredTable->setRowCount(0);
    int r = 0;
    for (const auto& ms : list) {
        m_expiredTable->insertRow(r);
        m_expiredTable->setItem(r, 0, new QTableWidgetItem(ms.getMemberName()));
        m_expiredTable->setItem(r, 1, new QTableWidgetItem(ms.getPlanName()));
        m_expiredTable->setItem(r, 2, new QTableWidgetItem(ms.getStartDate()));
        m_expiredTable->setItem(r, 3, new QTableWidgetItem(ms.getEndDate()));

        QWidget *actWidget = new QWidget(this);
        QHBoxLayout *actLayout = new QHBoxLayout(actWidget);
        actLayout->setContentsMargins(2, 2, 2, 2);
        actLayout->setAlignment(Qt::AlignCenter);

        QPushButton *renewBtn = new QPushButton("🔄", actWidget);
        renewBtn->setObjectName("iconSuccessBtn");
        renewBtn->setToolTip("Renew Membership Plan");
        renewBtn->setCursor(Qt::PointingHandCursor);
        renewBtn->setFixedSize(30, 28);
        connect(renewBtn, &QPushButton::clicked, this, &MembershipsWidget::onRenewClicked);
        m_expiredTable->setCellWidget(r, 4, actWidget);
        r++;
    }
}

void MembershipsWidget::loadPlansTable() {
    auto list = m_planRepo.findAll(false);
    m_plansTable->setRowCount(0);
    int r = 0;
    for (const auto& p : list) {
        m_plansTable->insertRow(r);
        m_plansTable->setItem(r, 0, new QTableWidgetItem(QString::number(p.getId())));
        m_plansTable->setItem(r, 1, new QTableWidgetItem(p.getPlanName()));
        m_plansTable->setItem(r, 2, new QTableWidgetItem(QString("%1 days").arg(p.getDurationDays())));
        m_plansTable->setItem(r, 3, new QTableWidgetItem(QString("Rs. %1").arg(p.getPrice(), 0, 'f', 0)));
        m_plansTable->setItem(r, 4, new QTableWidgetItem(p.getAccessType()));
        r++;
    }
}

void MembershipsWidget::onRenewClicked() {
    QDialog dlg(this);
    dlg.setWindowTitle("Membership Renewal / Assignment Modal");
    dlg.resize(500, 440);
    dlg.setMinimumSize(400, 350);
    dlg.setStyleSheet(ThemeManager::getLightThemeQss());
    dlg.setWindowFlags(Qt::Window | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);

    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    QFormLayout *form = new QFormLayout();

    QComboBox *memberCombo = new QComboBox(&dlg);
    auto members = m_memberRepo.findAll();
    for (const auto& m : members) {
        memberCombo->addItem(m.getFullName() + " (" + m.getMembershipNumber() + ")", m.getId());
    }

    QComboBox *planCombo = new QComboBox(&dlg);
    auto plans = m_planRepo.findAll(true);
    for (const auto& p : plans) {
        planCombo->addItem(QString("%1 — %2 days (Rs. %3)").arg(p.getPlanName()).arg(p.getDurationDays()).arg(p.getPrice(), 0, 'f', 0), p.getId());
    }

    QDoubleSpinBox *discountSpin = new QDoubleSpinBox(&dlg);
    discountSpin->setRange(0.0, 50000.0);
    discountSpin->setPrefix("Rs. ");

    QComboBox *methodCombo = new QComboBox(&dlg);
    methodCombo->addItems({"Cash", "Card", "Bank Transfer", "Other"});

    form->addRow("Select Member *:", memberCombo);
    form->addRow("Select Plan *:", planCombo);
    form->addRow("Discount Amount:", discountSpin);
    form->addRow("Payment Method *:", methodCombo);

    layout->addLayout(form);

    QHBoxLayout *btnBox = new QHBoxLayout();
    QPushButton *cancelBtn = new QPushButton("Cancel", &dlg);
    cancelBtn->setObjectName("secondaryBtn");
    QPushButton *confirmBtn = new QPushButton("Process Renewal & Receipt", &dlg);
    confirmBtn->setObjectName("successBtn");

    btnBox->addWidget(cancelBtn);
    btnBox->addWidget(confirmBtn);
    layout->addLayout(btnBox);

    connect(cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(confirmBtn, &QPushButton::clicked, [&]() {
        if (memberCombo->currentIndex() < 0 || planCombo->currentIndex() < 0) {
            QMessageBox::warning(&dlg, "Validation Error", "Please select a member and a plan.");
            return;
        }

        int memberId = memberCombo->currentData().toInt();
        int planId = planCombo->currentData().toInt();
        double discount = discountSpin->value();
        QString method = methodCombo->currentText();

        ServiceResult res = m_membershipService.renewMembership(memberId, planId, discount, method, 1);

        if (res.success) {
            dlg.accept();
            ToastNotification::show(this->window(), res.message, ToastType::Success);
            refreshData();
        } else {
            QMessageBox::critical(&dlg, "Renewal Failed", res.message);
        }
    });

    dlg.exec();
}

void MembershipsWidget::onAddPlanClicked() {
    QDialog dlg(this);
    dlg.setWindowTitle("Create New Membership Plan");
    dlg.resize(480, 380);
    dlg.setMinimumSize(380, 300);
    dlg.setStyleSheet(ThemeManager::getLightThemeQss());
    dlg.setWindowFlags(Qt::Window | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);

    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    QFormLayout *form = new QFormLayout();

    QLineEdit *nameInput = new QLineEdit(&dlg);
    QSpinBox *daysSpin = new QSpinBox(&dlg);
    daysSpin->setRange(1, 1095);
    daysSpin->setValue(30);

    QDoubleSpinBox *priceSpin = new QDoubleSpinBox(&dlg);
    priceSpin->setRange(0.0, 500000.0);
    priceSpin->setValue(3500.0);
    priceSpin->setPrefix("Rs. ");

    QComboBox *accessCombo = new QComboBox(&dlg);
    accessCombo->addItems({"Gym Floor", "All-Access", "VIP All-Access"});

    QLineEdit *descInput = new QLineEdit(&dlg);

    form->addRow("Plan Name *:", nameInput);
    form->addRow("Duration (Days) *:", daysSpin);
    form->addRow("Price (PKR) *:", priceSpin);
    form->addRow("Access Type:", accessCombo);
    form->addRow("Description:", descInput);

    layout->addLayout(form);

    QHBoxLayout *btnBox = new QHBoxLayout();
    QPushButton *cancelBtn = new QPushButton("Cancel", &dlg);
    cancelBtn->setObjectName("secondaryBtn");
    QPushButton *saveBtn = new QPushButton("Save Plan", &dlg);
    saveBtn->setObjectName("successBtn");

    btnBox->addWidget(cancelBtn);
    btnBox->addWidget(saveBtn);
    layout->addLayout(btnBox);

    connect(cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, [&]() {
        if (nameInput->text().trimmed().isEmpty()) {
            QMessageBox::warning(&dlg, "Validation Error", "Plan name is required.");
            return;
        }

        MembershipPlan plan;
        plan.setPlanName(nameInput->text().trimmed());
        plan.setDurationDays(daysSpin->value());
        plan.setPrice(priceSpin->value());
        plan.setAccessType(accessCombo->currentText());
        plan.setDescription(descInput->text().trimmed());
        plan.setStatus("Active");

        if (m_planRepo.create(plan)) {
            dlg.accept();
            ToastNotification::show(this->window(), "Membership plan created successfully: " + plan.getPlanName(), ToastType::Success);
            refreshData();
        } else {
            QMessageBox::critical(&dlg, "Database Error", "Failed to insert new membership plan.");
        }
    });

    dlg.exec();
}

void MembershipsWidget::onCancelMembershipClicked(int membershipId) {
    m_membershipService.cancelMembership(membershipId, 1);
    refreshData();
}

} // namespace FitCore
