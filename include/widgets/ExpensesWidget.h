#ifndef EXPENSESWIDGET_H
#define EXPENSESWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include "repositories/ExpenseRepository.h"

namespace FitCore {

class ExpensesWidget : public QWidget {
    Q_OBJECT
public:
    explicit ExpensesWidget(QWidget *parent = nullptr);

    void refreshData();

private slots:
    void onAddExpenseClicked();

private:
    void setupUi();
    void loadExpensesTable();

    QComboBox *m_categoryFilter;
    QTableWidget *m_table;

    ExpenseRepository m_expenseRepo;
};

} // namespace FitCore

#endif // EXPENSESWIDGET_H
