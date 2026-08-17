#include "services/MembershipService.h"
#include "database/DatabaseManager.h"
#include <QDate>
#include <QDebug>

namespace FitCore {

ServiceResult MembershipService::renewMembership(int memberId, int planId, double discountAmount, const QString& paymentMethod, int currentUserId) {
    ServiceResult res;

    MemberRepository memberRepo;
    MembershipPlanRepository planRepo;
    MembershipRepository membershipRepo;
    PaymentRepository paymentRepo;
    AuditRepository auditRepo;

    auto memberOpt = memberRepo.findById(memberId);
    if (!memberOpt.has_value()) {
        res.message = "Selected member does not exist.";
        return res;
    }

    auto planOpt = planRepo.findById(planId);
    if (!planOpt.has_value()) {
        res.message = "Selected membership plan does not exist.";
        return res;
    }

    MembershipPlan plan = planOpt.value();
    if (discountAmount < 0.0) {
        res.message = "Discount amount cannot be negative.";
        return res;
    }
    if (discountAmount > plan.getPrice()) {
        res.message = "Discount amount cannot exceed plan price.";
        return res;
    }

    QDate startDate = QDate::currentDate();
    // If active membership exists and hasn't expired yet, start new plan right after current end date
    auto activeOpt = membershipRepo.findActiveByMemberId(memberId);
    if (activeOpt.has_value()) {
        QDate currEndDate = QDate::fromString(activeOpt->getEndDate(), "yyyy-MM-dd");
        if (currEndDate.isValid() && currEndDate >= startDate) {
            startDate = currEndDate.addDays(1);
        }
    }

    QDate endDate = startDate.addDays(plan.getDurationDays());
    double finalAmount = plan.getPrice() - discountAmount;

    // Begin Database Transaction
    if (!DatabaseManager::instance().beginTransaction()) {
        res.message = "Failed to start database transaction for membership renewal.";
        return res;
    }

    Membership ms;
    ms.setMemberId(memberId);
    ms.setPlanId(planId);
    ms.setStartDate(startDate.toString("yyyy-MM-dd"));
    ms.setEndDate(endDate.toString("yyyy-MM-dd"));
    ms.setPrice(plan.getPrice());
    ms.setDiscount(discountAmount);
    ms.setFinalAmount(finalAmount);
    ms.setStatus("Active");
    ms.setPaymentStatus("Paid");
    ms.setCreatedBy(currentUserId);

    if (!membershipRepo.create(ms)) {
        DatabaseManager::instance().rollback();
        res.message = "Failed to insert new membership record into database.";
        return res;
    }

    Payment payment;
    payment.setMemberId(memberId);
    payment.setMembershipId(ms.getId());
    payment.setAmount(plan.getPrice());
    payment.setDiscount(discountAmount);
    payment.setFinalAmount(finalAmount);
    payment.setPaymentMethod(paymentMethod);
    payment.setPaymentDate(QDate::currentDate().toString("yyyy-MM-dd"));
    payment.setReceivedBy(currentUserId);
    payment.setNotes("Membership Renewal: " + plan.getPlanName());

    if (!paymentRepo.create(payment)) {
        DatabaseManager::instance().rollback();
        res.message = "Failed to record payment transaction.";
        return res;
    }

    // Ensure member status is active
    if (!memberRepo.setStatus(memberId, "Active")) {
        DatabaseManager::instance().rollback();
        res.message = "Failed to update member status.";
        return res;
    }

    auditRepo.logAction(currentUserId, "RENEW_MEMBERSHIP", "Membership", ms.getId(),
                        "Renewed membership for " + memberOpt->getFullName() + " with plan " + plan.getPlanName() + ". Receipt: " + payment.getReceiptNumber());

    if (DatabaseManager::instance().commit()) {
        res.success = true;
        res.entityId = ms.getId();
        res.message = QString("Membership renewed successfully! Receipt: %1, Valid until %2.")
                          .arg(payment.getReceiptNumber(), endDate.toString("dd-MMM-yyyy"));
        return res;
    }

    DatabaseManager::instance().rollback();
    res.message = "Transaction commit failed. No changes were saved.";
    return res;
}

ServiceResult MembershipService::cancelMembership(int membershipId, int currentUserId) {
    ServiceResult res;
    MembershipRepository repo;
    AuditRepository auditRepo;

    auto opt = repo.findById(membershipId);
    if (!opt.has_value()) {
        res.message = "Membership record not found.";
        return res;
    }

    if (repo.setStatus(membershipId, "Cancelled")) {
        auditRepo.logAction(currentUserId, "CANCEL_MEMBERSHIP", "Membership", membershipId, "Cancelled membership ID " + QString::number(membershipId));
        res.success = true;
        res.message = "Membership cancelled successfully.";
        return res;
    }
    res.message = "Failed to update membership status.";
    return res;
}

void MembershipService::updateAutoExpiries() {
    MembershipRepository repo;
    repo.autoUpdateExpiredStatuses();
}

} // namespace FitCore
