#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include "models/User.h"
#include "widgets/DashboardWidget.h"
#include "widgets/MembersWidget.h"
#include "widgets/MembershipsWidget.h"
#include "widgets/TrainersWidget.h"
#include "widgets/AttendanceWidget.h"
#include "widgets/PaymentsWidget.h"
#include "widgets/WorkoutWidget.h"
#include "widgets/ProgressWidget.h"
#include "widgets/EquipmentWidget.h"
#include "widgets/ExpensesWidget.h"
#include "widgets/ReportsWidget.h"
#include "widgets/NotificationsWidget.h"
#include "widgets/SettingsWidget.h"
#include "widgets/AuditLogsWidget.h"

namespace FitCore {

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(const User& currentUser, QWidget *parent = nullptr);
    ~MainWindow() override = default;

signals:
    void logoutRequested();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onSidebarSelectionChanged(int row);
    void onQuickCheckInSubmitted();
    void onLogoutClicked();
    void onToggleSidebarClicked();

private:
    void setupUi();
    void applyRoleRestrictions();

    User m_currentUser;

    QFrame *m_sidebarFrame;
    QLabel *m_logoLbl;
    QLabel *m_subLogoLbl;
    QPushButton *m_toggleSidebarBtn;
    bool m_sidebarCollapsed = false;
    bool m_loggingOut = false;

    QListWidget *m_sidebarList;
    QStackedWidget *m_stackedWidget;

    QLabel *m_userLabel;
    QLabel *m_roleBadge;
    QLineEdit *m_headerQuickCheckIn;

    // View Widgets
    DashboardWidget *m_dashboardWidget;
    MembersWidget *m_membersWidget;
    MembershipsWidget *m_membershipsWidget;
    TrainersWidget *m_trainersWidget;
    AttendanceWidget *m_attendanceWidget;
    PaymentsWidget *m_paymentsWidget;
    WorkoutWidget *m_workoutWidget;
    ProgressWidget *m_progressWidget;
    EquipmentWidget *m_equipmentWidget;
    ExpensesWidget *m_expensesWidget;
    ReportsWidget *m_reportsWidget;
    NotificationsWidget *m_notificationsWidget;
    SettingsWidget *m_settingsWidget;
    AuditLogsWidget *m_auditLogsWidget;
};

} // namespace FitCore

#endif // MAINWINDOW_H
