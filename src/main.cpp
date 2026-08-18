#include <QApplication>
#include <QMessageBox>
#include <iostream>
#include "database/DatabaseManager.h"
#include "services/MembershipService.h"
#include "services/NotificationService.h"
#include "services/AuthenticationService.h"
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

    // 3. Native C++ Desktop Application Flow (100% C++ Qt GUI)
    bool running = true;
    while (running) {
        std::cout << "[MAIN] Opening Native C++ LoginWindow Desktop Dialog..." << std::endl;
        FitCore::LoginWindow loginWindow;
        if (loginWindow.exec() == QDialog::Accepted) {
            auto userOpt = loginWindow.getAuthenticatedUser();
            if (userOpt.has_value()) {
                std::cout << "[MAIN] Login accepted. Launching Native C++ MainWindow..." << std::endl;
                FitCore::MainWindow mainWindow(userOpt.value());
                mainWindow.showMaximized();

                app.exec(); // Run Qt desktop event loop for main window

                // Check if user logged out or closed app window
                if (FitCore::AuthenticationService::instance().currentUser().has_value()) {
                    // User closed main window directly -> exit app
                    running = false;
                } else {
                    // User clicked Logout -> re-open LoginWindow dialog
                    std::cout << "[MAIN] User logged out. Re-opening Native C++ LoginWindow..." << std::endl;
                }
            } else {
                running = false;
            }
        } else {
            // User cancelled login or clicked Exit
            std::cout << "[MAIN] Login dialog closed or exited. Quitting application." << std::endl;
            running = false;
        }
    }

    std::cout << "[MAIN] Application shutdown completed cleanly." << std::endl;
    return 0;
}
