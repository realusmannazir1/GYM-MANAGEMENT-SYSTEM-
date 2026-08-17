#ifndef EXPENSE_H
#define EXPENSE_H

#include <QString>

namespace FitCore {

class Expense {
public:
    Expense();

    int getId() const;
    void setId(int id);

    QString getCategory() const;
    void setCategory(const QString& category);

    QString getDescription() const;
    void setDescription(const QString& desc);

    double getAmount() const;
    void setAmount(double amount);

    QString getExpenseDate() const;
    void setExpenseDate(const QString& date);

    QString getPaymentMethod() const;
    void setPaymentMethod(const QString& method);

    int getAddedBy() const;
    void setAddedBy(int userId);

    QString getNotes() const;
    void setNotes(const QString& notes);

private:
    int m_id;
    QString m_category;
    QString m_description;
    double m_amount;
    QString m_expenseDate;
    QString m_paymentMethod;
    int m_addedBy;
    QString m_notes;
};

} // namespace FitCore

#endif // EXPENSE_H
