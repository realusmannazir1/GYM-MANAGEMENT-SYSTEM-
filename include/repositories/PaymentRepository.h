#ifndef PAYMENTREPOSITORY_H
#define PAYMENTREPOSITORY_H

#include "models/Payment.h"
#include <optional>
#include <vector>

namespace FitCore {

class PaymentRepository {
public:
    PaymentRepository() = default;

    std::optional<Payment> findById(int id);
    std::optional<Payment> findByReceiptNumber(const QString& receiptNumber);
    std::vector<Payment> findByMemberId(int memberId);
    std::vector<Payment> findAllPaged(int limit, int offset);
    std::vector<Payment> findByDateRange(const QString& startDate, const QString& endDate);

    bool create(Payment& payment);
    QString generateNextReceiptNumber();

    double calculateMemberTotalPayments(int memberId);
    double getTodayRevenue();
    double getMonthlyRevenue();
    double getTotalRevenue();
};

} // namespace FitCore

#endif // PAYMENTREPOSITORY_H
