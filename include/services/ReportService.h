#ifndef REPORTSERVICE_H
#define REPORTSERVICE_H

#include <QString>
#include <map>
#include <vector>

namespace FitCore {

struct DashboardKPIs {
    int totalMembers{0};
    int activeMembers{0};
    int expiredMembers{0};
    int todayAttendance{0};
    int monthlyAttendance{0};
    int activeTrainers{0};
    int activeMemberships{0};
    int expiringSoonCount{0};
    double todayRevenue{0.0};
    double monthlyRevenue{0.0};
    double outstandingPayments{0.0};
    double monthlyExpenses{0.0};
    double monthlyProfit{0.0};
};

struct FinancialSummary {
    double totalRevenue{0.0};
    double totalExpenses{0.0};
    double netProfit{0.0};
    std::map<QString, double> revenueByMethod;
    std::map<QString, double> expensesByCategory;
};

class ReportService {
public:
    ReportService() = default;

    DashboardKPIs getDashboardKPIs();
    FinancialSummary getFinancialSummary(const QString& startDate = "", const QString& endDate = "");
    QString generateCSVReport(const QString& reportType, const QString& startDate = "", const QString& endDate = "");
};

} // namespace FitCore

#endif // REPORTSERVICE_H
