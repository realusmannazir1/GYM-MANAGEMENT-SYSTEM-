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
    QFrame* createStatCard(const QString& title, const QString& initialValue, const QString& iconStr);

    QLabel *m_totalMembersVal;
    QLabel *m_activeMembersVal;
    QLabel *m_expiredMembersVal;
    QLabel *m_todayAttendanceVal;
    QLabel *m_monthlyAttendanceVal;
    QLabel *m_activeTrainersVal;
    QLabel *m_activeMembershipsVal;
    QLabel *m_expiringSoonVal;
    QLabel *m_todayRevenueVal;
    QLabel *m_monthlyRevenueVal;
    QLabel *m_outstandingVal;
    QLabel *m_monthlyProfitVal;

    QTableWidget *m_expiringTable;
    QTableWidget *m_recentPaymentsTable;
    QTableWidget *m_recentCheckInsTable;

    ReportService m_reportService;
};

} // namespace FitCore

#endif // DASHBOARDWIDGET_H
