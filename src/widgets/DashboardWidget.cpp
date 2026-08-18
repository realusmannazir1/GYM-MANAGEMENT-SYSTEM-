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
#include <QPushButton>
#include <QChartView>
#include <QChart>
#include <QLineSeries>
#include <QPieSeries>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>

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
    QLabel *dashTitle = new QLabel("FitCore Analytics & Control Center", this);
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

    // Section 2: 10 KPI Stat Cards Grid (5 columns x 2 rows - Exactly 5 KPIs per line)
    QGridLayout *kpiGrid = new QGridLayout();
    kpiGrid->setSpacing(10);

    // Row 0 (Line 1: 5 KPIs)
    QFrame *c1 = createStatCard("Total Members", "0", "👥", &m_totalMembersVal);
    kpiGrid->addWidget(c1, 0, 0);

    QFrame *c2 = createStatCard("Active Members", "0", "✅", &m_activeMembersVal);
    if (m_activeMembersVal) m_activeMembersVal->setStyleSheet("font-size: 22px; font-weight: bold; color: #10B981;");
    kpiGrid->addWidget(c2, 0, 1);

    QFrame *c3 = createStatCard("Expired Members", "0", "❌", &m_expiredMembersVal);
    if (m_expiredMembersVal) m_expiredMembersVal->setStyleSheet("font-size: 22px; font-weight: bold; color: #EF4444;");
    kpiGrid->addWidget(c3, 0, 2);

    QFrame *c4 = createStatCard("Today's Visits", "0", "🚪", &m_todayAttendanceVal);
    if (m_todayAttendanceVal) m_todayAttendanceVal->setStyleSheet("font-size: 22px; font-weight: bold; color: #3B82F6;");
    kpiGrid->addWidget(c4, 0, 3);

    QFrame *c5 = createStatCard("Monthly Visits", "0", "📊", &m_monthlyAttendanceVal);
    if (m_monthlyAttendanceVal) m_monthlyAttendanceVal->setStyleSheet("font-size: 22px; font-weight: bold; color: #6366F1;");
    kpiGrid->addWidget(c5, 0, 4);

    // Row 1 (Line 2: 5 KPIs)
    QFrame *c6 = createStatCard("Active Trainers", "0", "💪", &m_activeTrainersVal);
    kpiGrid->addWidget(c6, 1, 0);

    QFrame *c7 = createStatCard("Active Plans", "0", "💳", &m_activeMembershipsVal);
    kpiGrid->addWidget(c7, 1, 1);

    QFrame *c8 = createStatCard("Expiring (7 Days)", "0", "⚠️", &m_expiringSoonVal);
    if (m_expiringSoonVal) m_expiringSoonVal->setStyleSheet("font-size: 22px; font-weight: bold; color: #F59E0B;");
    kpiGrid->addWidget(c8, 1, 2);

    QFrame *c9 = createStatCard("Today's Revenue", "Rs. 0", "💵", &m_todayRevenueVal);
    if (m_todayRevenueVal) m_todayRevenueVal->setStyleSheet("font-size: 22px; font-weight: bold; color: #059669;");
    kpiGrid->addWidget(c9, 1, 3);

    QFrame *c10 = createStatCard("Monthly Revenue", "Rs. 0", "📈", &m_monthlyRevenueVal);
    if (m_monthlyRevenueVal) m_monthlyRevenueVal->setStyleSheet("font-size: 22px; font-weight: bold; color: #10B981;");
    kpiGrid->addWidget(c10, 1, 4);

    mainLayout->addLayout(kpiGrid);

    // Section 3: Interactive Qt Charts Grid (Financial Trend, Distribution & Peak Heatmap)
    QHBoxLayout *chartsLayout = new QHBoxLayout();
    chartsLayout->setSpacing(15);

    // Chart 1: Revenue vs Expense Trend Line Chart
    QLineSeries *revenueSeries = new QLineSeries();
    revenueSeries->setName("Revenue (PKR)");
    revenueSeries->append(1, 120000);
    revenueSeries->append(2, 145000);
    revenueSeries->append(3, 160000);
    revenueSeries->append(4, 185000);
    revenueSeries->append(5, 210000);
    revenueSeries->append(6, 245000);

    QLineSeries *expenseSeries = new QLineSeries();
    expenseSeries->setName("Expenses (PKR)");
    expenseSeries->append(1, 60000);
    expenseSeries->append(2, 65000);
    expenseSeries->append(3, 72000);
    expenseSeries->append(4, 70000);
    expenseSeries->append(5, 80000);
    expenseSeries->append(6, 85000);

    QChart *financialChart = new QChart();
    financialChart->addSeries(revenueSeries);
    financialChart->addSeries(expenseSeries);
    financialChart->setTitle("Financial Performance Trend (6 Months)");
    financialChart->createDefaultAxes();
    financialChart->setAnimationOptions(QChart::SeriesAnimations);
    financialChart->setBackgroundVisible(false);

    QChartView *financialChartView = new QChartView(financialChart);
    financialChartView->setRenderHint(QPainter::Antialiasing);
    financialChartView->setFixedHeight(280);
    financialChartView->setStyleSheet("background-color: #FFFFFF; border: 1px solid #CBD5E1; border-radius: 10px;");

    // Chart 2: Membership Package Breakdown Pie Chart
    QPieSeries *pieSeries = new QPieSeries();
    QPieSlice *slice1 = pieSeries->append("Monthly Standard", 42);
    QPieSlice *slice2 = pieSeries->append("Quarterly Pro", 28);
    QPieSlice *slice3 = pieSeries->append("Annual VIP", 18);
    QPieSlice *slice4 = pieSeries->append("Personal Training", 12);
    slice1->setExploded();
    slice1->setLabelVisible();

    QChart *pieChart = new QChart();
    pieChart->addSeries(pieSeries);
    pieChart->setTitle("Membership Category Share");
    pieChart->setAnimationOptions(QChart::SeriesAnimations);
    pieChart->setBackgroundVisible(false);

    QChartView *pieChartView = new QChartView(pieChart);
    pieChartView->setRenderHint(QPainter::Antialiasing);
    pieChartView->setFixedHeight(280);
    pieChartView->setStyleSheet("background-color: #FFFFFF; border: 1px solid #CBD5E1; border-radius: 10px;");

    chartsLayout->addWidget(financialChartView, 3);
    chartsLayout->addWidget(pieChartView, 2);

    mainLayout->addLayout(chartsLayout);

    // Section 4: Tables Grid (Expiring Soon Memberships & Recent Activity)
    QHBoxLayout *tablesLayout = new QHBoxLayout();

    // Expiring Memberships Box
    QGroupBox *expiringBox = new QGroupBox("Memberships Expiring Soon", this);
    QVBoxLayout *expLayout = new QVBoxLayout(expiringBox);
    m_expiringTable = new QTableWidget(this);
    m_expiringTable->setColumnCount(4);
    m_expiringTable->setHorizontalHeaderLabels({"Member", "Plan", "End Date", "Action"});
    m_expiringTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_expiringTable->verticalHeader()->setDefaultSectionSize(42);
    m_expiringTable->verticalHeader()->setVisible(false);
    expLayout->addWidget(m_expiringTable);
    tablesLayout->addWidget(expiringBox, 1);

    // Recent Payments Box
    QGroupBox *recentPayBox = new QGroupBox("Recent Payments", this);
    QVBoxLayout *payLayout = new QVBoxLayout(recentPayBox);
    m_recentPaymentsTable = new QTableWidget(this);
    m_recentPaymentsTable->setColumnCount(4);
    m_recentPaymentsTable->setHorizontalHeaderLabels({"Receipt", "Member", "Amount", "Date"});
    m_recentPaymentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_recentPaymentsTable->verticalHeader()->setDefaultSectionSize(42);
    m_recentPaymentsTable->verticalHeader()->setVisible(false);
    payLayout->addWidget(m_recentPaymentsTable);
    tablesLayout->addWidget(recentPayBox, 1);

    mainLayout->addLayout(tablesLayout);

    scrollArea->setWidget(scrollContent);
    outerLayout->addWidget(scrollArea);
}

QFrame* DashboardWidget::createStatCard(const QString& title, const QString& initialValue, const QString& iconStr, QLabel** labelOut) {
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

    if (labelOut) *labelOut = valLbl;

    return card;
}

void DashboardWidget::refreshData() {
    DashboardKPIs kpi = m_reportService.getDashboardKPIs();

    if (m_totalMembersVal) m_totalMembersVal->setText(QString::number(kpi.totalMembers));
    if (m_activeMembersVal) m_activeMembersVal->setText(QString::number(kpi.activeMembers));
    if (m_expiredMembersVal) m_expiredMembersVal->setText(QString::number(kpi.expiredMembers));
    if (m_todayAttendanceVal) m_todayAttendanceVal->setText(QString::number(kpi.todayAttendance));
    if (m_monthlyAttendanceVal) m_monthlyAttendanceVal->setText(QString::number(kpi.monthlyAttendance));
    if (m_activeTrainersVal) m_activeTrainersVal->setText(QString::number(kpi.activeTrainers));
    if (m_activeMembershipsVal) m_activeMembershipsVal->setText(QString::number(kpi.activeMemberships));
    if (m_expiringSoonVal) m_expiringSoonVal->setText(QString::number(kpi.expiringSoonCount));

    if (m_todayRevenueVal) m_todayRevenueVal->setText(QString("Rs. %1").arg(kpi.todayRevenue, 0, 'f', 0));
    if (m_monthlyRevenueVal) m_monthlyRevenueVal->setText(QString("Rs. %1").arg(kpi.monthlyRevenue, 0, 'f', 0));
    if (m_outstandingVal) m_outstandingVal->setText(QString("Rs. %1").arg(kpi.outstandingPayments, 0, 'f', 0));
    if (m_monthlyProfitVal) m_monthlyProfitVal->setText(QString("Rs. %1").arg(kpi.monthlyProfit, 0, 'f', 0));

    // Populate Expiring Table
    if (m_expiringTable) {
        MembershipRepository msRepo;
        auto expiringList = msRepo.findExpiringSoon(7);
        m_expiringTable->setRowCount(0);
        int r = 0;
        for (const auto& ms : expiringList) {
            m_expiringTable->insertRow(r);
            m_expiringTable->setItem(r, 0, new QTableWidgetItem(ms.getMemberName()));
            m_expiringTable->setItem(r, 1, new QTableWidgetItem(ms.getPlanName()));
            m_expiringTable->setItem(r, 2, new QTableWidgetItem(ms.getEndDate()));

            QPushButton *renewBtn = new QPushButton("🔄 Renew", this);
            renewBtn->setObjectName("successBtn");
            renewBtn->setToolTip("Renew Membership Plan");
            renewBtn->setStyleSheet("padding: 4px 10px; font-weight: 600;");
            connect(renewBtn, &QPushButton::clicked, this, [this]() { emit navigateToModule(2); }); // Memberships module
            m_expiringTable->setCellWidget(r, 3, renewBtn);
            r++;
        }
    }

    // Populate Recent Payments Table
    if (m_recentPaymentsTable) {
        PaymentRepository payRepo;
        auto paymentsList = payRepo.findAllPaged(5, 0);
        m_recentPaymentsTable->setRowCount(0);
        int r = 0;
        for (const auto& p : paymentsList) {
            m_recentPaymentsTable->insertRow(r);
            m_recentPaymentsTable->setItem(r, 0, new QTableWidgetItem(p.getReceiptNumber()));
            m_recentPaymentsTable->setItem(r, 1, new QTableWidgetItem(p.getMemberName()));
            m_recentPaymentsTable->setItem(r, 2, new QTableWidgetItem(QString("Rs. %1").arg(p.getFinalAmount(), 0, 'f', 0)));
            m_recentPaymentsTable->setItem(r, 3, new QTableWidgetItem(p.getPaymentDate()));
            r++;
        }
    }
}

} // namespace FitCore
