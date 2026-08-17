#ifndef PAYMENT_H
#define PAYMENT_H

#include <QString>

namespace FitCore {

class Payment {
public:
    Payment();

    int getId() const;
    void setId(int id);

    QString getReceiptNumber() const;
    void setReceiptNumber(const QString& receiptNumber);

    int getMemberId() const;
    void setMemberId(int memberId);

    QString getMemberName() const;
    void setMemberName(const QString& name);

    int getMembershipId() const;
    void setMembershipId(int membershipId);

    double getAmount() const;
    void setAmount(double amount);

    double getDiscount() const;
    void setDiscount(double discount);

    double getFinalAmount() const;
    void setFinalAmount(double amount);

    QString getPaymentMethod() const;
    void setPaymentMethod(const QString& method);

    QString getPaymentDate() const;
    void setPaymentDate(const QString& date);

    int getReceivedBy() const;
    void setReceivedBy(int userId);

    QString getNotes() const;
    void setNotes(const QString& notes);

private:
    int m_id;
    QString m_receiptNumber;
    int m_memberId;
    QString m_memberName;
    int m_membershipId;
    double m_amount;
    double m_discount;
    double m_finalAmount;
    QString m_paymentMethod;
    QString m_paymentDate;
    int m_receivedBy;
    QString m_notes;
};

} // namespace FitCore

#endif // PAYMENT_H
