#ifndef DASHBOARDWIDGET_H
#define DASHBOARDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include "services/ReportService.h"

namespace FitCore {

class DashboardWidget : public QWidget {
    Q_OBJECT
public:
    explicit DashboardWidget(QWidget *parent = nullptr);

    void refreshData();

signals:
    void navigateToModule(int moduleIndex);

private:
    void setupUi();
    QFrame* createStatCard(const QString& title, const QString& initialValue, const QString& iconStr, QLabel** labelOut = nullptr);

    QLabel *m_totalMembersVal = nullptr;
    QLabel *m_activeMembersVal = nullptr;
    QLabel *m_expiredMembersVal = nullptr;
    QLabel *m_todayAttendanceVal = nullptr;
    QLabel *m_monthlyAttendanceVal = nullptr;
    QLabel *m_activeTrainersVal = nullptr;
    QLabel *m_activeMembershipsVal = nullptr;
    QLabel *m_expiringSoonVal = nullptr;
    QLabel *m_todayRevenueVal = nullptr;
    QLabel *m_monthlyRevenueVal = nullptr;
    QLabel *m_outstandingVal = nullptr;
    QLabel *m_monthlyProfitVal = nullptr;

    QTableWidget *m_expiringTable = nullptr;
    QTableWidget *m_recentPaymentsTable = nullptr;
    QTableWidget *m_recentCheckInsTable = nullptr;

    ReportService m_reportService;
};

} // namespace FitCore

#endif // DASHBOARDWIDGET_H
