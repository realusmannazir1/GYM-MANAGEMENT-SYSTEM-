#include <QApplication>
#include <QMessageBox>
#include <iostream>
#include "database/DatabaseManager.h"
#include "services/MembershipService.h"
#include "services/NotificationService.h"
#include "widgets/LoginWindow.h"
#include "widgets/MainWindow.h"
#include "utils/ThemeManager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("FitCore Gym Management System");
    app.setOrganizationName("FitCore Software");
    app.setApplicationVersion("1.0.0");

    std::cout << "[MAIN] App created." << std::endl;

    // Apply global stylesheet theme
    app.setStyleSheet(FitCore::ThemeManager::getLightThemeQss());
    std::cout << "[MAIN] Theme applied." << std::endl;

    // 1. Initialize SQLite Database Singleton
    std::cout << "[MAIN] Initializing database..." << std::endl;
    if (!FitCore::DatabaseManager::instance().initialize("database/fitcore.db")) {
        std::cout << "[MAIN] Database init FAILED." << std::endl;
        QMessageBox::critical(nullptr, "Database Error", "Failed to connect to SQLite database.\nPlease check file permissions.");
        return 1;
    }
    std::cout << "[MAIN] Database initialized OK." << std::endl;

    // 2. Perform automated maintenance background tasks
    std::cout << "[MAIN] Running MembershipService::updateAutoExpiries()..." << std::endl;
    FitCore::MembershipService membershipService;
    membershipService.updateAutoExpiries();
    std::cout << "[MAIN] MembershipService done." << std::endl;

    std::cout << "[MAIN] Running NotificationService::checkAndGenerateExpiriesAndAlerts()..." << std::endl;
    FitCore::NotificationService notificationService;
    notificationService.checkAndGenerateExpiriesAndAlerts();
    std::cout << "[MAIN] NotificationService done." << std::endl;

    // 3. Show Authentication Login Window
    std::cout << "[MAIN] Creating LoginWindow..." << std::endl;
    FitCore::LoginWindow loginWindow;
    std::cout << "[MAIN] Showing LoginWindow..." << std::endl;
    if (loginWindow.exec() == QDialog::Accepted) {
        auto userOpt = loginWindow.getAuthenticatedUser();
        if (userOpt.has_value()) {
            std::cout << "[MAIN] Login accepted, launching MainWindow in maximized mode..." << std::endl;
            FitCore::MainWindow mainWindow(userOpt.value());
            mainWindow.showMaximized();
            return app.exec();
        }
    }

    std::cout << "[MAIN] Login cancelled or no user. Exiting." << std::endl;
    return 0;
}
