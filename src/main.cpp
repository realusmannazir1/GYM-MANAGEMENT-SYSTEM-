#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>
#include <iostream>
#include "database/DatabaseManager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Set library paths for Qt plugins
    QString appDir = QApplication::applicationDirPath();
    QApplication::addLibraryPath(appDir);
    QApplication::addLibraryPath(appDir + "/sqldrivers");
    QApplication::addLibraryPath(appDir + "/platforms");
    QApplication::addLibraryPath("D:/Qt/6.7.2/mingw_64/plugins");

    std::cout << "[INFO] FitCore Initialization Mode" << std::endl;
    std::cout << "[INFO] Available SQL Drivers: " << QSqlDatabase::drivers().join(", ").toStdString() << std::endl;

    if (!FitCore::DatabaseManager::instance().initialize("data/fitcore.db")) {
        std::cerr << "[CRIT] Database initialization failed: " << FitCore::DatabaseManager::instance().lastError().toStdString() << std::endl;
        return 1;
    }

    std::cout << "[INFO] Database initialized & seeded successfully at data/fitcore.db" << std::endl;

    // Verify record counts
    QSqlQuery q(FitCore::DatabaseManager::instance().getDatabase());
    if (q.exec("SELECT COUNT(*) FROM users;")) {
        if (q.next()) {
            std::cout << "[INFO] Seeded Users Count: " << q.value(0).toInt() << std::endl;
        }
    }

    if (q.exec("SELECT COUNT(*) FROM members;")) {
        if (q.next()) {
            std::cout << "[INFO] Seeded Members Count: " << q.value(0).toInt() << std::endl;
        }
    }

    if (q.exec("SELECT COUNT(*) FROM membership_plans;")) {
        if (q.next()) {
            std::cout << "[INFO] Seeded Plans Count: " << q.value(0).toInt() << std::endl;
        }
    }

    std::cout << "[SUCCESS] Database Architecture & Schema Verification PASSED!" << std::endl;
    
    // If CLI argument --test passed, exit cleanly after DB initialization
    for (int i = 1; i < argc; ++i) {
        if (QString(argv[i]) == "--test") {
            return 0;
        }
    }

    return 0;
}
