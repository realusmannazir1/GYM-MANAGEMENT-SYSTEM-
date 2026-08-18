#include <QApplication>
#include <QMessageBox>
#include "database/DatabaseManager.h"
#include "services/MembershipService.h"
#include "services/NotificationService.h"
#include "widgets/LoginWindow.h"
#include "widgets/MainWindow.h"
#include "utils/ThemeManager.h"

int main(int argc, char *argv[]) {
    QCoreApplication::addLibraryPath("D:/Qt/6.7.2/mingw_64/plugins");
    QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath() + "/plugins");
    QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath());

    QApplication app(argc, argv);
    app.setApplicationName("FitCore Gym Management System");
    app.setOrganizationName("FitCore Software");
    app.setApplicationVersion("1.0.0");

    // Apply global stylesheet theme
    app.setStyleSheet(FitCore::ThemeManager::getDarkThemeQss());

    // 1. Initialize SQLite Database Singleton
    if (!FitCore::DatabaseManager::instance().initialize("database/fitcore.db")) {
        QMessageBox::critical(nullptr, "Database Error", "Failed to connect to SQLite database.\nPlease check file permissions.");
        return 1;
    }

    // 2. Perform automated maintenance background tasks
    FitCore::MembershipService membershipService;
    membershipService.updateAutoExpiries();

    FitCore::NotificationService notificationService;
    notificationService.checkAndGenerateExpiriesAndAlerts();

    // 3. Show Authentication Login Window
    FitCore::LoginWindow loginWindow;
    if (loginWindow.exec() == QDialog::Accepted) {
        auto userOpt = loginWindow.getAuthenticatedUser();
        if (userOpt.has_value()) {
            FitCore::MainWindow mainWindow(userOpt.value());
            mainWindow.show();
            return app.exec();
        }
    }

    return 0;
}
