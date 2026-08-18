#include "widgets/MainWindow.h"
#include "services/AuthenticationService.h"
#include "services/AttendanceService.h"
#include "utils/ThemeManager.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QMessageBox>
#include <QStatusBar>

namespace FitCore {

MainWindow::MainWindow(const User& currentUser, QWidget *parent)
    : QMainWindow(parent), m_currentUser(currentUser) {
    setWindowTitle("FitCore — Gym Management System [C++17 & Qt 6 & SQLite]");
    resize(1360, 850);
    setMinimumSize(960, 640);
    setStyleSheet(ThemeManager::getLightThemeQss());

    setupUi();
    applyRoleRestrictions();
}

void MainWindow::setupUi() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Section 1: Sidebar Frame
    m_sidebarFrame = new QFrame(this);
    m_sidebarFrame->setObjectName("sidebarFrame");
    m_sidebarFrame->setFixedWidth(240);

    QVBoxLayout *sbLayout = new QVBoxLayout(m_sidebarFrame);
    sbLayout->setContentsMargins(10, 20, 10, 20);
    sbLayout->setSpacing(15);

    // App Logo Banner
    m_logoLbl = new QLabel("FITCORE", m_sidebarFrame);
    m_logoLbl->setAlignment(Qt::AlignCenter);
    m_logoLbl->setStyleSheet("font-size: 26px; font-weight: 900; color: #1E40AF; letter-spacing: 2px;");

    m_subLogoLbl = new QLabel("MANAGEMENT SYSTEM", m_sidebarFrame);
    m_subLogoLbl->setAlignment(Qt::AlignCenter);
    m_subLogoLbl->setStyleSheet("font-size: 10px; font-weight: 700; color: #1E3A8A; letter-spacing: 1px;");

    sbLayout->addWidget(m_logoLbl);
    sbLayout->addWidget(m_subLogoLbl);

    m_sidebarList = new QListWidget(m_sidebarFrame);
    m_sidebarList->setObjectName("sidebarList");

    m_sidebarList->addItem("📊 Dashboard");
    m_sidebarList->addItem("👥 Members Directory");
    m_sidebarList->addItem("💳 Memberships & Renewals");
    m_sidebarList->addItem("💪 Trainers & PT");
    m_sidebarList->addItem("🚪 Attendance Terminal");
    m_sidebarList->addItem("🧾 Payments & Receipts");
    m_sidebarList->addItem("🏋️ Workout Plans");
    m_sidebarList->addItem("📈 Body Progress");
    m_sidebarList->addItem("⚙️ Equipment Assets");
    m_sidebarList->addItem("💸 Expense Outflow");
    m_sidebarList->addItem("📄 Reports & Analytics");
    m_sidebarList->addItem("🔔 Notification Alerts");
    m_sidebarList->addItem("🛠️ System Settings");
    m_sidebarList->addItem("📋 Audit Logs");

    m_sidebarList->setCurrentRow(0);
    sbLayout->addWidget(m_sidebarList);

    QPushButton *logoutBtn = new QPushButton("Logout", m_sidebarFrame);
    logoutBtn->setObjectName("secondaryBtn");
    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);
    sbLayout->addWidget(logoutBtn);

    mainLayout->addWidget(m_sidebarFrame);

    // Section 2: Right Content Container (Header + QStackedWidget)
    QWidget *rightContainer = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightContainer);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);

    // Top Header Bar Frame
    QFrame *headerFrame = new QFrame(this);
    headerFrame->setObjectName("headerFrame");
    headerFrame->setFixedHeight(65);

    QHBoxLayout *headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(15, 0, 20, 0);

    m_toggleSidebarBtn = new QPushButton("☰ Menu", headerFrame);
    m_toggleSidebarBtn->setObjectName("secondaryBtn");
    m_toggleSidebarBtn->setToolTip("Toggle Collapsible Navigation Sidebar (Open/Close)");
    m_toggleSidebarBtn->setCursor(Qt::PointingHandCursor);
    m_toggleSidebarBtn->setStyleSheet("font-size: 13px; font-weight: bold; padding: 6px 12px; border-radius: 6px;");
    connect(m_toggleSidebarBtn, &QPushButton::clicked, this, &MainWindow::onToggleSidebarClicked);

    QLabel *headerTitle = new QLabel("FitCore Enterprise Dashboard", headerFrame);
    headerTitle->setObjectName("headerTitle");

    m_headerQuickCheckIn = new QLineEdit(headerFrame);
    m_headerQuickCheckIn->setPlaceholderText("Quick Check-In (Scan barcode or enter MEM #)...");
    m_headerQuickCheckIn->setFixedWidth(320);

    m_userLabel = new QLabel(m_currentUser.getFullName(), headerFrame);
    m_userLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #0F172A;");

    m_roleBadge = new QLabel(m_currentUser.getRoleName(), headerFrame);
    m_roleBadge->setObjectName("roleBadge");

    headerLayout->addWidget(m_toggleSidebarBtn);
    headerLayout->addSpacing(10);
    headerLayout->addWidget(headerTitle);
    headerLayout->addStretch();
    headerLayout->addWidget(m_headerQuickCheckIn);
    headerLayout->addSpacing(20);
    headerLayout->addWidget(m_userLabel);
    headerLayout->addWidget(m_roleBadge);

    rightLayout->addWidget(headerFrame);

    // Central Stacked Widget
    m_stackedWidget = new QStackedWidget(this);

    m_dashboardWidget = new DashboardWidget(this);
    m_membersWidget = new MembersWidget(this);
    m_membershipsWidget = new MembershipsWidget(this);
    m_trainersWidget = new TrainersWidget(this);
    m_attendanceWidget = new AttendanceWidget(this);
    m_paymentsWidget = new PaymentsWidget(this);
    m_workoutWidget = new WorkoutWidget(this);
    m_progressWidget = new ProgressWidget(this);
    m_equipmentWidget = new EquipmentWidget(this);
    m_expensesWidget = new ExpensesWidget(this);
    m_reportsWidget = new ReportsWidget(this);
    m_notificationsWidget = new NotificationsWidget(this);
    m_settingsWidget = new SettingsWidget(this);
    m_auditLogsWidget = new AuditLogsWidget(this);

    m_stackedWidget->addWidget(m_dashboardWidget);
    m_stackedWidget->addWidget(m_membersWidget);
    m_stackedWidget->addWidget(m_membershipsWidget);
    m_stackedWidget->addWidget(m_trainersWidget);
    m_stackedWidget->addWidget(m_attendanceWidget);
    m_stackedWidget->addWidget(m_paymentsWidget);
    m_stackedWidget->addWidget(m_workoutWidget);
    m_stackedWidget->addWidget(m_progressWidget);
    m_stackedWidget->addWidget(m_equipmentWidget);
    m_stackedWidget->addWidget(m_expensesWidget);
    m_stackedWidget->addWidget(m_reportsWidget);
    m_stackedWidget->addWidget(m_notificationsWidget);
    m_stackedWidget->addWidget(m_settingsWidget);
    m_stackedWidget->addWidget(m_auditLogsWidget);

    rightLayout->addWidget(m_stackedWidget);
    mainLayout->addWidget(rightContainer);

    statusBar()->showMessage("FitCore System Ready. Connected to SQLite Database.");

    // Signal/Slot Connects
    connect(m_sidebarList, &QListWidget::currentRowChanged, this, &MainWindow::onSidebarSelectionChanged);
    connect(m_dashboardWidget, &DashboardWidget::navigateToModule, this, [this](int idx) {
        m_sidebarList->setCurrentRow(idx);
    });
    connect(m_headerQuickCheckIn, &QLineEdit::returnPressed, this, &MainWindow::onQuickCheckInSubmitted);
}

void MainWindow::applyRoleRestrictions() {
    UserRole role = m_currentUser.getRole();
    if (role == UserRole::Receptionist) {
        // Receptionist: hide Settings (12) and Audit Logs (13)
        m_sidebarList->item(12)->setHidden(true);
        m_sidebarList->item(13)->setHidden(true);
    } else if (role == UserRole::Trainer) {
        // Trainer: hide Payments (5), Expenses (9), Settings (12), Audit Logs (13)
        m_sidebarList->item(5)->setHidden(true);
        m_sidebarList->item(9)->setHidden(true);
        m_sidebarList->item(12)->setHidden(true);
        m_sidebarList->item(13)->setHidden(true);
    }
}

void MainWindow::onSidebarSelectionChanged(int row) {
    if (row < 0 || row >= m_stackedWidget->count()) return;
    m_stackedWidget->setCurrentIndex(row);

    // Refresh widget data dynamically on view switch
    QWidget *curr = m_stackedWidget->currentWidget();
    if (curr == m_dashboardWidget) m_dashboardWidget->refreshData();
    else if (curr == m_membersWidget) m_membersWidget->refreshData();
    else if (curr == m_membershipsWidget) m_membershipsWidget->refreshData();
    else if (curr == m_trainersWidget) m_trainersWidget->refreshData();
    else if (curr == m_attendanceWidget) m_attendanceWidget->refreshData();
    else if (curr == m_paymentsWidget) m_paymentsWidget->refreshData();
    else if (curr == m_workoutWidget) m_workoutWidget->refreshData();
    else if (curr == m_progressWidget) m_progressWidget->refreshData();
    else if (curr == m_equipmentWidget) m_equipmentWidget->refreshData();
    else if (curr == m_expensesWidget) m_expensesWidget->refreshData();
    else if (curr == m_reportsWidget) m_reportsWidget->refreshData();
    else if (curr == m_notificationsWidget) m_notificationsWidget->refreshData();
    else if (curr == m_settingsWidget) m_settingsWidget->refreshData();
    else if (curr == m_auditLogsWidget) m_auditLogsWidget->refreshData();
}

void MainWindow::onQuickCheckInSubmitted() {
    QString text = m_headerQuickCheckIn->text().trimmed();
    if (text.isEmpty()) return;

    AttendanceService attService;
    CheckInResult res = attService.checkInByMembershipNumber(text, m_currentUser.getId());

    if (res.success) {
        QMessageBox::information(this, "Check-In Success", res.message);
        m_headerQuickCheckIn->clear();
        m_sidebarList->setCurrentRow(4); // Switch to attendance terminal
    } else {
        QMessageBox::warning(this, "Check-In Alert", res.message);
    }
}

void MainWindow::onLogoutClicked() {
    AuthenticationService::instance().logout();
    close();
}

void MainWindow::onToggleSidebarClicked() {
    m_sidebarCollapsed = !m_sidebarCollapsed;

    if (m_sidebarCollapsed) {
        m_sidebarFrame->setFixedWidth(68);
        m_logoLbl->setText("FC");
        m_subLogoLbl->hide();
        m_toggleSidebarBtn->setText("▶ Open Menu");
        m_toggleSidebarBtn->setToolTip("Expand Navigation Sidebar");
    } else {
        m_sidebarFrame->setFixedWidth(240);
        m_logoLbl->setText("FITCORE");
        m_subLogoLbl->show();
        m_toggleSidebarBtn->setText("☰ Menu");
        m_toggleSidebarBtn->setToolTip("Collapse Navigation Sidebar");
    }
}

} // namespace FitCore
