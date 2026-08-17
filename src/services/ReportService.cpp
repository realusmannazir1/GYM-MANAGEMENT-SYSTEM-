#include "services/ReportService.h"
#include "repositories/MemberRepository.h"
#include "repositories/MembershipRepository.h"
#include "repositories/AttendanceRepository.h"
#include "repositories/TrainerRepository.h"
#include "repositories/PaymentRepository.h"
#include "repositories/ExpenseRepository.h"
#include "database/DatabaseManager.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDate>
#include <QTextStream>

namespace FitCore {

DashboardKPIs ReportService::getDashboardKPIs() {
    DashboardKPIs kpi;
    MemberRepository memberRepo;
    MembershipRepository msRepo;
    AttendanceRepository attRepo;
    TrainerRepository trainerRepo;
    PaymentRepository paymentRepo;
    ExpenseRepository expenseRepo;

    kpi.totalMembers = memberRepo.count("All");
    kpi.activeMembers = memberRepo.count("Active");
    kpi.expiredMembers = memberRepo.count("Inactive") + memberRepo.count("Archived");

    kpi.todayAttendance = attRepo.getTodayVisitsCount();
    kpi.monthlyAttendance = attRepo.getMonthlyVisitsCount();

    kpi.activeTrainers = static_cast<int>(trainerRepo.findAll(true).size());
    kpi.activeMemberships = static_cast<int>(msRepo.findAll("Active").size());
    kpi.expiringSoonCount = static_cast<int>(msRepo.findExpiringSoon(7).size());

    kpi.todayRevenue = paymentRepo.getTodayRevenue();
    kpi.monthlyRevenue = paymentRepo.getMonthlyRevenue();
    kpi.monthlyExpenses = expenseRepo.getMonthlyExpenses();
    kpi.monthlyProfit = kpi.monthlyRevenue - kpi.monthlyExpenses;

    // Calculate outstanding payments (Sum of price - final_amount for pending or unpaid memberships)
    QSqlQuery query("SELECT SUM(price - final_amount) FROM memberships WHERE payment_status != 'Paid';", DatabaseManager::instance().getDatabase());
    if (query.next()) {
        kpi.outstandingPayments = query.value(0).toDouble();
    }

    return kpi;
}

FinancialSummary ReportService::getFinancialSummary(const QString& startDate, const QString& endDate) {
    FinancialSummary summary;
    PaymentRepository paymentRepo;
    ExpenseRepository expenseRepo;

    if (startDate.isEmpty() || endDate.isEmpty()) {
        summary.totalRevenue = paymentRepo.getTotalRevenue();
        summary.totalExpenses = expenseRepo.getTotalExpenses();
    } else {
        auto payments = paymentRepo.findByDateRange(startDate, endDate);
        for (const auto& p : payments) {
            summary.totalRevenue += p.getFinalAmount();
            summary.revenueByMethod[p.getPaymentMethod()] += p.getFinalAmount();
        }

        auto expenses = expenseRepo.findByDateRange(startDate, endDate);
        for (const auto& e : expenses) {
            summary.totalExpenses += e.getAmount();
            summary.expensesByCategory[e.getCategory()] += e.getAmount();
        }
    }
    summary.netProfit = summary.totalRevenue - summary.totalExpenses;
    return summary;
}

QString ReportService::generateCSVReport(const QString& reportType, const QString& startDate, const QString& endDate) {
    QString csv;
    QTextStream out(&csv);

    if (reportType == "Members") {
        out << "ID,Membership Number,First Name,Last Name,Gender,Phone,Email,Status,Registration Date\n";
        MemberRepository repo;
        auto members = repo.findAll();
        for (const auto& m : members) {
            out << m.getId() << ","
                << "\"" << m.getMembershipNumber() << "\","
                << "\"" << m.getFirstName() << "\","
                << "\"" << m.getLastName() << "\","
                << "\"" << m.getGender() << "\","
                << "\"" << m.getPhone() << "\","
                << "\"" << m.getEmail() << "\","
                << "\"" << m.getProfileStatus() << "\","
                << "\"" << m.getRegistrationDate() << "\"\n";
        }
    } else if (reportType == "Payments") {
        out << "Receipt Number,Member Name,Amount,Discount,Final Amount,Method,Payment Date\n";
        PaymentRepository repo;
        auto payments = (startDate.isEmpty() || endDate.isEmpty()) ? repo.findAllPaged(1000, 0) : repo.findByDateRange(startDate, endDate);
        for (const auto& p : payments) {
            out << "\"" << p.getReceiptNumber() << "\","
                << "\"" << p.getMemberName() << "\","
                << p.getAmount() << ","
                << p.getDiscount() << ","
                << p.getFinalAmount() << ","
                << "\"" << p.getPaymentMethod() << "\","
                << "\"" << p.getPaymentDate() << "\"\n";
        }
    } else if (reportType == "Attendance") {
        out << "Member Name,Membership Number,Date,Check-In,Check-Out,Duration\n";
        AttendanceRepository repo;
        auto list = (startDate.isEmpty() || endDate.isEmpty()) ? repo.findAllPaged(1000, 0) : repo.findByDateRange(startDate, endDate);
        for (const auto& a : list) {
            out << "\"" << a.getMemberName() << "\","
                << "\"" << a.getMembershipNumber() << "\","
                << "\"" << a.getAttendanceDate() << "\","
                << "\"" << a.getCheckInTime() << "\","
                << "\"" << a.getCheckOutTime() << "\","
                << "\"" << a.getDurationFormatted() << "\"\n";
        }
    }

    return csv;
}

} // namespace FitCore
