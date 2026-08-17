#include "database/DatabaseManager.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>

namespace FitCore {

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::~DatabaseManager() {
    close();
}

bool DatabaseManager::initialize(const QString& dbPath) {
    std::lock_guard<std::mutex> lock(m_mutex);

    m_dbPath = dbPath;
    m_connectionName = "FitCoreConnection";

    QFileInfo fileInfo(m_dbPath);
    QDir dir = fileInfo.dir();
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            m_lastError = "Failed to create database directory: " + dir.path();
            qCritical() << m_lastError;
            return false;
        }
    }

    bool dbExists = fileInfo.exists();

    if (!open()) {
        return false;
    }

    // Execute schema if new database or tables missing
    QSqlQuery checkQuery("SELECT name FROM sqlite_master WHERE type='table' AND name='users';", getDatabase());
    bool needSchema = !checkQuery.next();

    if (needSchema) {
        qInfo() << "Initializing SQLite Database Schema from schema.sql...";
        if (!executeScriptFile("database/schema.sql")) {
            qWarning() << "Failed to run schema.sql directly, attempting fallback path...";
            if (!executeScriptFile("schema.sql")) {
                m_lastError = "Failed to execute database schema creation script.";
                qCritical() << m_lastError;
                return false;
            }
        }

        qInfo() << "Seeding initial database records from seed.sql...";
        executeScriptFile("database/seed.sql");
    }

    return true;
}

bool DatabaseManager::open() {
    if (QSqlDatabase::contains(m_connectionName)) {
        QSqlDatabase db = QSqlDatabase::database(m_connectionName);
        if (db.isOpen()) {
            return true;
        }
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);
    db.setDatabaseName(m_dbPath);

    if (!db.open()) {
        m_lastError = db.lastError().text();
        qCritical() << "Database open error:" << m_lastError;
        return false;
    }

    // Enable foreign key constraints and WAL journal mode
    QSqlQuery query(db);
    query.exec("PRAGMA foreign_keys = ON;");
    query.exec("PRAGMA journal_mode = WAL;");

    qInfo() << "SQLite Database opened successfully:" << m_dbPath;
    return true;
}

void DatabaseManager::close() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (QSqlDatabase::contains(m_connectionName)) {
        {
            QSqlDatabase db = QSqlDatabase::database(m_connectionName);
            if (db.isOpen()) {
                db.close();
            }
        }
        QSqlDatabase::removeDatabase(m_connectionName);
    }
}

bool DatabaseManager::isOpen() const {
    if (QSqlDatabase::contains(m_connectionName)) {
        return QSqlDatabase::database(m_connectionName).isOpen();
    }
    return false;
}

QSqlDatabase DatabaseManager::getDatabase() {
    if (!isOpen()) {
        open();
    }
    return QSqlDatabase::database(m_connectionName);
}

bool DatabaseManager::execute(const QString& sql) {
    std::lock_guard<std::mutex> lock(m_mutex);
    QSqlQuery query(getDatabase());
    if (!query.exec(sql)) {
        m_lastError = query.lastError().text();
        qCritical() << "SQL Execution Failure:" << m_lastError << "\nQuery:" << sql;
        return false;
    }
    return true;
}

bool DatabaseManager::executeScriptFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_lastError = "Cannot open script file: " + filePath;
        qWarning() << m_lastError;
        return false;
    }

    QTextStream in(&file);
    QString script = in.readAll();
    file.close();

    QStringList statements = script.split(';', Qt::SkipEmptyParts);
    QSqlDatabase db = getDatabase();

    db.transaction();
    for (QString statement : statements) {
        statement = statement.trimmed();
        if (statement.isEmpty() || statement.startsWith("--")) {
            continue;
        }

        QSqlQuery query(db);
        if (!query.exec(statement)) {
            m_lastError = query.lastError().text();
            qWarning() << "Script Statement Error:" << m_lastError << "\nStatement:" << statement;
            db.rollback();
            return false;
        }
    }
    return db.commit();
}

QSqlQuery DatabaseManager::prepare(const QString& sql) {
    QSqlQuery query(getDatabase());
    query.prepare(sql);
    return query;
}

bool DatabaseManager::beginTransaction() {
    return getDatabase().transaction();
}

bool DatabaseManager::commit() {
    return getDatabase().commit();
}

bool DatabaseManager::rollback() {
    return getDatabase().rollback();
}

QString DatabaseManager::lastError() const {
    return m_lastError;
}

} // namespace FitCore
