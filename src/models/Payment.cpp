#include "models/Payment.h"

namespace FitCore {

Payment::Payment()
    : m_id(0), m_memberId(0), m_membershipId(0), m_amount(0.0),
      m_discount(0.0), m_finalAmount(0.0), m_paymentMethod("Cash"), m_receivedBy(1) {}

int Payment::getId() const { return m_id; }
void Payment::setId(int id) { m_id = id; }

QString Payment::getReceiptNumber() const { return m_receiptNumber; }
void Payment::setReceiptNumber(const QString& receiptNumber) { m_receiptNumber = receiptNumber; }

int Payment::getMemberId() const { return m_memberId; }
void Payment::setMemberId(int memberId) { m_memberId = memberId; }

QString Payment::getMemberName() const { return m_memberName; }
void Payment::setMemberName(const QString& name) { m_memberName = name; }

int Payment::getMembershipId() const { return m_membershipId; }
void Payment::setMembershipId(int membershipId) { m_membershipId = membershipId; }

double Payment::getAmount() const { return m_amount; }
void Payment::setAmount(double amount) { m_amount = amount; }

double Payment::getDiscount() const { return m_discount; }
void Payment::setDiscount(double discount) { m_discount = discount; }

double Payment::getFinalAmount() const { return m_finalAmount; }
void Payment::setFinalAmount(double amount) { m_finalAmount = amount; }

QString Payment::getPaymentMethod() const { return m_paymentMethod; }
void Payment::setPaymentMethod(const QString& method) { m_paymentMethod = method; }

QString Payment::getPaymentDate() const { return m_paymentDate; }
void Payment::setPaymentDate(const QString& date) { m_paymentDate = date; }

int Payment::getReceivedBy() const { return m_receivedBy; }
void Payment::setReceivedBy(int userId) { m_receivedBy = userId; }

QString Payment::getNotes() const { return m_notes; }
void Payment::setNotes(const QString& notes) { m_notes = notes; }

} // namespace FitCore
