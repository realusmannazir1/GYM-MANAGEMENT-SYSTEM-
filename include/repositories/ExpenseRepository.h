#ifndef EXPENSEREPOSITORY_H
#define EXPENSEREPOSITORY_H

#include "models/Expense.h"
#include <optional>
#include <vector>
#include <map>

namespace FitCore {

class ExpenseRepository {
public:
    ExpenseRepository() = default;

    std::optional<Expense> findById(int id);
    std::vector<Expense> findAll(const QString& categoryFilter = "");
    std::vector<Expense> findByDateRange(const QString& startDate, const QString& endDate);

    bool create(Expense& expense);
    bool remove(int id);

    double getMonthlyExpenses();
    double getTotalExpenses();
    std::map<QString, double> getCategoryExpenseBreakdown();
};

} // namespace FitCore

#endif // EXPENSEREPOSITORY_H
