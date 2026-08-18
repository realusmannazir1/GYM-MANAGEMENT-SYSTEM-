#include <QApplication>
#include <QMessageBox>
#include <QSqlDatabase>
#include <fstream>
#include "database/DatabaseManager.h"
#include "services/MembershipService.h"
#include "services/NotificationService.h"
#include "widgets/LoginWindow.h"
#include "widgets/MainWindow.h"
#include "utils/ThemeManager.h"

int main(int argc, char *argv[]) {
    std::ofstream log("launch_debug.txt");
    log << "Starting FitCore..." << std::endl;
    log.flush();

    log << "Initializing QApplication..." << std::endl;
    log.flush();
    QApplication app(argc, argv);

    log << "QApplication initialized successfully." << std::endl;
    log.flush();

    log << "Adding library paths..." << std::endl;
    log.flush();
    QCoreApplication::addLibraryPath("D:/Qt/6.7.2/mingw_64/plugins");
    QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath() + "/plugins");
    QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath());

    log << "Available SQL drivers: ";
    for (const QString& driver : QSqlDatabase::drivers()) {
        log << driver.toStdString() << " ";
    }
    log << std::endl;
    log.flush();

    app.setApplicationName("FitCore Gym Management System");
    app.setOrganizationName("FitCore Software");
    app.setApplicationVersion("1.0.0");

    // Apply global stylesheet theme
    log << "Setting stylesheet..." << std::endl;
    log.flush();
    app.setStyleSheet(FitCore::ThemeManager::getDarkThemeQss());
    log << "Stylesheet set." << std::endl;
    log.flush();

    // 1. Initialize SQLite Database Singleton
    log << "Initializing database..." << std::endl;
    log.flush();
    if (!FitCore::DatabaseManager::instance().initialize("database/fitcore.db")) {
        log << "Database init failed! Error: " << FitCore::DatabaseManager::instance().lastError().toStdString() << std::endl;
        log.flush();
        QMessageBox::critical(nullptr, "Database Error", "Failed to connect to SQLite database.\nPlease check file permissions.");
        return 1;
    }
    log << "Database initialized successfully." << std::endl;
    log.flush();

    // 2. Perform automated maintenance background tasks
    log << "Performing maintenance..." << std::endl;
    log.flush();
    FitCore::MembershipService membershipService;
    membershipService.updateAutoExpiries();

    FitCore::NotificationService notificationService;
    notificationService.checkAndGenerateExpiriesAndAlerts();
    log << "Maintenance tasks completed." << std::endl;
    log.flush();

    // 3. Show Authentication Login Window
    log << "Showing LoginWindow..." << std::endl;
    log.flush();
    FitCore::LoginWindow loginWindow;
    int res = loginWindow.exec();
    log << "LoginWindow result: " << res << std::endl;
    log.flush();

    if (res == QDialog::Accepted) {
        auto userOpt = loginWindow.getAuthenticatedUser();
        if (userOpt.has_value()) {
            log << "Showing MainWindow..." << std::endl;
            log.flush();
            FitCore::MainWindow mainWindow(userOpt.value());
            mainWindow.show();
            return app.exec();
        }
    }

    log << "Exiting main with 0." << std::endl;
    log.flush();
    return 0;
}
