#include "widgets/DashboardWidget.h"
#include "repositories/MembershipRepository.h"
#include "repositories/PaymentRepository.h"
#include "repositories/AttendanceRepository.h"
#include "utils/ThemeManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QGroupBox>
#include <QScrollArea>

namespace FitCore {

DashboardWidget::DashboardWidget(QWidget *parent)
    : QWidget(parent) {
    setupUi();
    refreshData();
}

void DashboardWidget::setupUi() {
    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(15, 15, 15, 15);
    outerLayout->setSpacing(15);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget *scrollContent = new QWidget(scrollArea);
    QVBoxLayout *mainLayout = new QVBoxLayout(scrollContent);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(20);

    // Section 1: Top Header Banner & Quick Actions
    QHBoxLayout *bannerLayout = new QHBoxLayout();
    QLabel *dashTitle = new QLabel("Dashboard Overview", this);
    dashTitle->setStyleSheet("font-size: 22px; font-weight: bold; color: #0F172A;");

    QPushButton *quickCheckInBtn = new QPushButton("+ Check-In Member", this);
    quickCheckInBtn->setObjectName("successBtn");
    connect(quickCheckInBtn, &QPushButton::clicked, this, [this]() { emit navigateToModule(4); }); // Attendance module

    QPushButton *quickAddMemberBtn = new QPushButton("+ Register Member", this);
    connect(quickAddMemberBtn, &QPushButton::clicked, this, [this]() { emit navigateToModule(1); }); // Members module

    bannerLayout->addWidget(dashTitle);
    bannerLayout->addStretch();
    bannerLayout->addWidget(quickAddMemberBtn);
    bannerLayout->addWidget(quickCheckInBtn);
    mainLayout->addLayout(bannerLayout);

    // Section 2: 12 KPI Stat Cards Grid (4 columns x 3 rows)
    QGridLayout *kpiGrid = new QGridLayout();
    kpiGrid->setSpacing(12);

    // Row 0: Members & Attendance
    QFrame *c1 = createStatCard("Total Members", "0", "[M]");
    m_totalMembersVal = c1->findChild<QLabel*>("statValue");
    kpiGrid->addWidget(c1, 0, 0);

    QFrame *c2 = createStatCard("Active Members", "0", "[A]");
    m_activeMembersVal = c2->findChild<QLabel*>("statValue");
    m_activeMembersVal->setStyleSheet("font-size: 24px; font-weight: bold; color: #10B981;");
    kpiGrid->addWidget(c2, 0, 1);

    QFrame *c3 = createStatCard("Expired / Inactive", "0", "[E]");
    m_expiredMembersVal = c3->findChild<QLabel*>("statValue");
    m_expiredMembersVal->setStyleSheet("font-size: 24px; font-weight: bold; color: #EF4444;");
    kpiGrid->addWidget(c3, 0, 2);

    QFrame *c4 = createStatCard("Today's Check-ins", "0", "[V]");
    m_todayAttendanceVal = c4->findChild<QLabel*>("statValue");
    m_todayAttendanceVal->setStyleSheet("font-size: 24px; font-weight: bold; color: #3B82F6;");
    kpiGrid->addWidget(c4, 0, 3);

    // Row 1: Operations & Memberships
    QFrame *c5 = createStatCard("Monthly Visits", "0", "[MV]");
    m_monthlyAttendanceVal = c5->findChild<QLabel*>("statValue");
    kpiGrid->addWidget(c5, 1, 0);

    QFrame *c6 = createStatCard("Active Trainers", "0", "[TR]");
    m_activeTrainersVal = c6->findChild<QLabel*>("statValue");
    kpiGrid->addWidget(c6, 1, 1);

    QFrame *c7 = createStatCard("Active Memberships", "0", "[MS]");
    m_activeMembershipsVal = c7->findChild<QLabel*>("statValue");
    kpiGrid->addWidget(c7, 1, 2);

    QFrame *c8 = createStatCard("Expiring (7 Days)", "0", "[EX]");
    m_expiringSoonVal = c8->findChild<QLabel*>("statValue");
    m_expiringSoonVal->setStyleSheet("font-size: 24px; font-weight: bold; color: #F59E0B;");
    kpiGrid->addWidget(c8, 1, 3);

    // Row 2: Financial KPIs
    QFrame *c9 = createStatCard("Today's Revenue", "Rs. 0", "[REV]");
    m_todayRevenueVal = c9->findChild<QLabel*>("statValue");
    kpiGrid->addWidget(c9, 2, 0);

    QFrame *c10 = createStatCard("Monthly Revenue", "Rs. 0", "[MREV]");
    m_monthlyRevenueVal = c10->findChild<QLabel*>("statValue");
    m_monthlyRevenueVal->setStyleSheet("font-size: 24px; font-weight: bold; color: #10B981;");
    kpiGrid->addWidget(c10, 2, 1);

    QFrame *c11 = createStatCard("Outstanding Balance", "Rs. 0", "[BAL]");
    m_outstandingVal = c11->findChild<QLabel*>("statValue");
    m_outstandingVal->setStyleSheet("font-size: 24px; font-weight: bold; color: #F59E0B;");
    kpiGrid->addWidget(c11, 2, 2);

    QFrame *c12 = createStatCard("Est. Monthly Profit", "Rs. 0", "[PFT]");
    m_monthlyProfitVal = c12->findChild<QLabel*>("statValue");
    m_monthlyProfitVal->setStyleSheet("font-size: 24px; font-weight: bold; color: #3B82F6;");
    kpiGrid->addWidget(c12, 2, 3);

    mainLayout->addLayout(kpiGrid);

    // Section 3: Tables Grid (Expiring Soon Memberships & Recent Activity)
    QHBoxLayout *tablesLayout = new QHBoxLayout();

    // Expiring Memberships Box
    QGroupBox *expiringBox = new QGroupBox("Memberships Expiring Soon", this);
    QVBoxLayout *expLayout = new QVBoxLayout(expiringBox);
    m_expiringTable = new QTableWidget(this);
    m_expiringTable->setColumnCount(4);
    m_expiringTable->setHorizontalHeaderLabels({"Member", "Plan", "End Date", "Action"});
    m_expiringTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    expLayout->addWidget(m_expiringTable);
    tablesLayout->addWidget(expiringBox, 1);

    // Recent Payments Box
    QGroupBox *recentPayBox = new QGroupBox("Recent Payments", this);
    QVBoxLayout *payLayout = new QVBoxLayout(recentPayBox);
    m_recentPaymentsTable = new QTableWidget(this);
    m_recentPaymentsTable->setColumnCount(4);
    m_recentPaymentsTable->setHorizontalHeaderLabels({"Receipt", "Member", "Amount", "Date"});
    m_recentPaymentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    payLayout->addWidget(m_recentPaymentsTable);
    tablesLayout->addWidget(recentPayBox, 1);

    mainLayout->addLayout(tablesLayout);

    scrollArea->setWidget(scrollContent);
    outerLayout->addWidget(scrollArea);
}

QFrame* DashboardWidget::createStatCard(const QString& title, const QString& initialValue, const QString& iconStr) {
    QFrame *card = new QFrame(this);
    card->setObjectName("statCard");

    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(4);

    QHBoxLayout *topRow = new QHBoxLayout();
    QLabel *titleLbl = new QLabel(title, card);
    titleLbl->setObjectName("statTitle");

    QLabel *iconLbl = new QLabel(iconStr, card);
    iconLbl->setStyleSheet("color: #64748B; font-weight: bold;");

    topRow->addWidget(titleLbl);
    topRow->addStretch();
    topRow->addWidget(iconLbl);

    QLabel *valLbl = new QLabel(initialValue, card);
    valLbl->setObjectName("statValue");

    layout->addLayout(topRow);
    layout->addWidget(valLbl);

    return card;
}

void DashboardWidget::refreshData() {
    DashboardKPIs kpi = m_reportService.getDashboardKPIs();

    m_totalMembersVal->setText(QString::number(kpi.totalMembers));
    m_activeMembersVal->setText(QString::number(kpi.activeMembers));
    m_expiredMembersVal->setText(QString::number(kpi.expiredMembers));
    m_todayAttendanceVal->setText(QString::number(kpi.todayAttendance));
    m_monthlyAttendanceVal->setText(QString::number(kpi.monthlyAttendance));
    m_activeTrainersVal->setText(QString::number(kpi.activeTrainers));
    m_activeMembershipsVal->setText(QString::number(kpi.activeMemberships));
    m_expiringSoonVal->setText(QString::number(kpi.expiringSoonCount));

    m_todayRevenueVal->setText(QString("Rs. %1").arg(kpi.todayRevenue, 0, 'f', 0));
    m_monthlyRevenueVal->setText(QString("Rs. %1").arg(kpi.monthlyRevenue, 0, 'f', 0));
    m_outstandingVal->setText(QString("Rs. %1").arg(kpi.outstandingPayments, 0, 'f', 0));
    m_monthlyProfitVal->setText(QString("Rs. %1").arg(kpi.monthlyProfit, 0, 'f', 0));

    // Populate Expiring Table
    MembershipRepository msRepo;
    auto expiringList = msRepo.findExpiringSoon(7);
    m_expiringTable->setRowCount(0);
    int r = 0;
    for (const auto& ms : expiringList) {
        m_expiringTable->insertRow(r);
        m_expiringTable->setItem(r, 0, new QTableWidgetItem(ms.getMemberName()));
        m_expiringTable->setItem(r, 1, new QTableWidgetItem(ms.getPlanName()));
        m_expiringTable->setItem(r, 2, new QTableWidgetItem(ms.getEndDate()));

        QPushButton *renewBtn = new QPushButton("Renew", this);
        renewBtn->setStyleSheet("padding: 4px 8px; font-size: 11px;");
        connect(renewBtn, &QPushButton::clicked, this, [this]() { emit navigateToModule(2); }); // Memberships module
        m_expiringTable->setCellWidget(r, 3, renewBtn);
        r++;
    }

    // Populate Recent Payments Table
    PaymentRepository payRepo;
    auto paymentsList = payRepo.findAllPaged(5, 0);
    m_recentPaymentsTable->setRowCount(0);
    r = 0;
    for (const auto& p : paymentsList) {
        m_recentPaymentsTable->insertRow(r);
        m_recentPaymentsTable->setItem(r, 0, new QTableWidgetItem(p.getReceiptNumber()));
        m_recentPaymentsTable->setItem(r, 1, new QTableWidgetItem(p.getMemberName()));
        m_recentPaymentsTable->setItem(r, 2, new QTableWidgetItem(QString("Rs. %1").arg(p.getFinalAmount(), 0, 'f', 0)));
        m_recentPaymentsTable->setItem(r, 3, new QTableWidgetItem(p.getPaymentDate()));
        r++;
    }
}

} // namespace FitCore
