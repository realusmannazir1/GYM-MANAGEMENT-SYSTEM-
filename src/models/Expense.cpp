#include "models/Expense.h"

namespace FitCore {

Expense::Expense()
    : m_id(0), m_amount(0.0), m_paymentMethod("Cash"), m_addedBy(1) {}

int Expense::getId() const { return m_id; }
void Expense::setId(int id) { m_id = id; }

QString Expense::getCategory() const { return m_category; }
void Expense::setCategory(const QString& category) { m_category = category; }

QString Expense::getDescription() const { return m_description; }
void Expense::setDescription(const QString& desc) { m_description = desc; }

double Expense::getAmount() const { return m_amount; }
void Expense::setAmount(double amount) { m_amount = amount; }

QString Expense::getExpenseDate() const { return m_expenseDate; }
void Expense::setExpenseDate(const QString& date) { m_expenseDate = date; }

QString Expense::getPaymentMethod() const { return m_paymentMethod; }
void Expense::setPaymentMethod(const QString& method) { m_paymentMethod = method; }

int Expense::getAddedBy() const { return m_addedBy; }
void Expense::setAddedBy(int userId) { m_addedBy = userId; }

QString Expense::getNotes() const { return m_notes; }
void Expense::setNotes(const QString& notes) { m_notes = notes; }

} // namespace FitCore
