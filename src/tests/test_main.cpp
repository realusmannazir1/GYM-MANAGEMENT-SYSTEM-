#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include <cassert>
#include "database/DatabaseManager.h"
#include "services/AuthenticationService.h"
#include "services/MembershipService.h"
#include "services/AttendanceService.h"
#include "services/ReportService.h"
#include "services/BackupService.h"
#include "repositories/MemberRepository.h"
#include "repositories/MembershipPlanRepository.h"
#include "repositories/ProgressRepository.h"

using namespace FitCore;

void runTests() {
    std::cout << "\n======================================================\n";
    std::cout << "          FitCore Automated Test Suite Runner         \n";
    std::cout << "======================================================\n\n";

    int passed = 0;
    int total = 0;

    auto testAssert = [&](bool condition, const std::string& name) {
        total++;
        if (condition) {
            std::cout << " [PASS] Test " << total << ": " << name << "\n";
            passed++;
        } else {
            std::cout << " [FAIL] Test " << total << ": " << name << "\n";
        }
    };

    // Test 1: Database Initialization
    bool dbOk = DatabaseManager::instance().open();
    testAssert(dbOk, "DatabaseManager Connection & Auto Schema Initialization");

    // Test 2: Authentication Service Admin Login
    AuthResult authRes = AuthenticationService::instance().login("admin", "admin123");
    testAssert(authRes.success && authRes.user.has_value(), "AuthenticationService Valid Admin Login (admin/admin123)");

    // Test 3: Authentication Service Invalid Password Failure
    AuthResult failRes = AuthenticationService::instance().login("admin", "wrongpassword");
    testAssert(!failRes.success, "AuthenticationService Invalid Password Rejection");

    // Test 4: Member Code Auto-Generation
    MemberRepository memberRepo;
    QString nextMemNum = memberRepo.generateNextMembershipNumber();
    testAssert(nextMemNum.startsWith("MEM-"), "MemberRepository Membership Number Generation (" + nextMemNum.toStdString() + ")");

    // Test 5: Member Registration
    Member testMember;
    testMember.setMembershipNumber(nextMemNum);
    testMember.setFirstName("Test");
    testMember.setLastName("User");
    testMember.setGender("Male");
    testMember.setPhone("+92 300 9998877");
    testMember.setEmail("testuser@fitcore.com");
    testMember.setProfileStatus("Active");

    bool memberCreated = memberRepo.create(testMember);
    testAssert(memberCreated && testMember.getId() > 0, "MemberRepository Insert New Member Record");

    // Test 6: Membership Plan Assignment & Renewal Transaction
    MembershipService msService;
    ServiceResult renewRes = msService.renewMembership(testMember.getId(), 1, 0.0, "Cash", 1);
    testAssert(renewRes.success && renewRes.entityId > 0, "MembershipService Transactional Renewal & Receipt Creation");

    // Test 7: Attendance Check-In Rule
    AttendanceService attService;
    CheckInResult checkInRes = attService.checkInByMemberId(testMember.getId(), 1);
    testAssert(checkInRes.success, "AttendanceService Member Check-In Execution");

    // Test 8: Duplicate Check-In Protection
    CheckInResult dupRes = attService.checkInByMemberId(testMember.getId(), 1);
    testAssert(!dupRes.success, "AttendanceService Duplicate Check-In Prevention Rule");

    // Test 9: BMI Calculation Logic
    ProgressRecord pr;
    pr.setWeightKg(80.0);
    pr.setHeightCm(180.0);
    double bmi = pr.calculateBmi(); // 80 / (1.8 * 1.8) = 24.69
    testAssert(bmi > 24.5 && bmi < 24.8, "ProgressRecord Formula BMI Calculation (" + std::to_string(bmi) + ")");

    // Test 10: Dashboard KPI Generation
    ReportService reportService;
    DashboardKPIs kpi = reportService.getDashboardKPIs();
    testAssert(kpi.totalMembers > 0 && kpi.activeMembers > 0, "ReportService Dashboard KPI Aggregation");

    // Test 11: SQLite Hot Backup VACUUM INTO
    BackupService backupService;
    BackupResult backupRes = backupService.createBackup("backups/test_backup");
    testAssert(backupRes.success, "BackupService Live SQLite Hot Backup ('VACUUM INTO')");

    std::cout << "\n------------------------------------------------------\n";
    std::cout << " TEST RESULTS: " << passed << " / " << total << " PASSED (" << (passed * 100 / total) << "%)\n";
    std::cout << "======================================================\n\n";
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    runTests();
    return 0;
}
