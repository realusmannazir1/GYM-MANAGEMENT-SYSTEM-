#include "repositories/ExpenseRepository.h"
#include "database/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDate>

namespace FitCore {

static Expense mapExpense(const QSqlQuery& query) {
    Expense e;
    e.setId(query.value("expense_id").toInt());
    e.setCategory(query.value("category").toString());
    e.setDescription(query.value("description").toString());
    e.setAmount(query.value("amount").toDouble());
    e.setExpenseDate(query.value("expense_date").toString());
    e.setPaymentMethod(query.value("payment_method").toString());
    e.setAddedBy(query.value("added_by").toInt());
    e.setNotes(query.value("notes").toString());
    return e;
}

std::optional<Expense> ExpenseRepository::findById(int id) {
    QSqlQuery query = DatabaseManager::instance().prepare("SELECT * FROM expenses WHERE expense_id = ?;");
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        return mapExpense(query);
    }
    return std::nullopt;
}

std::vector<Expense> ExpenseRepository::findAll(const QString& categoryFilter) {
    std::vector<Expense> list;
    QString sql = "SELECT * FROM expenses ";
    if (!categoryFilter.isEmpty() && categoryFilter != "All") {
        sql += "WHERE category = '" + categoryFilter + "' ";
    }
    sql += "ORDER BY expense_id DESC;";

    QSqlQuery query(sql, DatabaseManager::instance().getDatabase());
    while (query.next()) {
        list.push_back(mapExpense(query));
    }
    return list;
}

std::vector<Expense> ExpenseRepository::findByDateRange(const QString& startDate, const QString& endDate) {
    std::vector<Expense> list;
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT * FROM expenses WHERE expense_date >= ? AND expense_date <= ? ORDER BY expense_date DESC;"
    );
    query.addBindValue(startDate);
    query.addBindValue(endDate);
    if (query.exec()) {
        while (query.next()) list.push_back(mapExpense(query));
    }
    return list;
}

bool ExpenseRepository::create(Expense& expense) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "INSERT INTO expenses (category, description, amount, expense_date, payment_method, added_by, notes) "
        "VALUES (?, ?, ?, ?, ?, ?, ?);"
    );
    query.addBindValue(expense.getCategory());
    query.addBindValue(expense.getDescription());
    query.addBindValue(expense.getAmount());
    query.addBindValue(expense.getExpenseDate().isEmpty() ? QDate::currentDate().toString("yyyy-MM-dd") : expense.getExpenseDate());
    query.addBindValue(expense.getPaymentMethod());
    query.addBindValue(expense.getAddedBy());
    query.addBindValue(expense.getNotes());

    if (query.exec()) {
        expense.setId(query.lastInsertId().toInt());
        return true;
    }
    return false;
}

bool ExpenseRepository::remove(int id) {
    QSqlQuery query = DatabaseManager::instance().prepare("DELETE FROM expenses WHERE expense_id = ?;");
    query.addBindValue(id);
    return query.exec();
}

double ExpenseRepository::getMonthlyExpenses() {
    QString monthPrefix = QDate::currentDate().toString("yyyy-MM-");
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT SUM(amount) FROM expenses WHERE expense_date LIKE ?;"
    );
    query.addBindValue(monthPrefix + "%");
    if (query.exec() && query.next()) {
        return query.value(0).toDouble();
    }
    return 0.0;
}

double ExpenseRepository::getTotalExpenses() {
    QSqlQuery query("SELECT SUM(amount) FROM expenses;", DatabaseManager::instance().getDatabase());
    if (query.next()) {
        return query.value(0).toDouble();
    }
    return 0.0;
}

std::map<QString, double> ExpenseRepository::getCategoryExpenseBreakdown() {
    std::map<QString, double> breakdown;
    QSqlQuery query("SELECT category, SUM(amount) AS total FROM expenses GROUP BY category;", DatabaseManager::instance().getDatabase());
    while (query.next()) {
        breakdown[query.value("category").toString()] = query.value("total").toDouble();
    }
    return breakdown;
}

} // namespace FitCore
