#include <QApplication>
#include <QDebug>
#include "database/DatabaseManager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    qInfo() << "=========================================";
    qInfo() << "   FitCore Gym Management System v1.0.0  ";
    qInfo() << "=========================================";

    if (!FitCore::DatabaseManager::instance().initialize("data/fitcore.db")) {
        qCritical() << "Fatal Error: Database initialization failed!";
        return 1;
    }

    qInfo() << "Database initialization and schema validation successful!";
    return 0;
}
