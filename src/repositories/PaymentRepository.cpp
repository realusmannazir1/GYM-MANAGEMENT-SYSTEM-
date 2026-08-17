#include "repositories/PaymentRepository.h"
#include "database/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include <QDate>
#include <QDebug>

namespace FitCore {

static Payment mapPayment(const QSqlQuery& query) {
    Payment p;
    p.setId(query.value("payment_id").toInt());
    p.setReceiptNumber(query.value("receipt_number").toString());
    p.setMemberId(query.value("member_id").toInt());
    p.setMembershipId(query.value("membership_id").toInt());
    p.setAmount(query.value("amount").toDouble());
    p.setDiscount(query.value("discount").toDouble());
    p.setFinalAmount(query.value("final_amount").toDouble());
    p.setPaymentMethod(query.value("payment_method").toString());
    p.setPaymentDate(query.value("payment_date").toString());
    p.setReceivedBy(query.value("received_by").toInt());
    p.setNotes(query.value("notes").toString());

    if (query.record().indexOf("member_name") >= 0) {
        p.setMemberName(query.value("member_name").toString());
    }
    return p;
}

std::optional<Payment> PaymentRepository::findById(int id) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT p.*, (m.first_name || ' ' || m.last_name) AS member_name "
        "FROM payments p JOIN members m ON p.member_id = m.member_id WHERE p.payment_id = ?;"
    );
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        return mapPayment(query);
    }
    return std::nullopt;
}

std::optional<Payment> PaymentRepository::findByReceiptNumber(const QString& receiptNumber) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT p.*, (m.first_name || ' ' || m.last_name) AS member_name "
        "FROM payments p JOIN members m ON p.member_id = m.member_id WHERE UPPER(p.receipt_number) = UPPER(?);"
    );
    query.addBindValue(receiptNumber.trimmed());
    if (query.exec() && query.next()) {
        return mapPayment(query);
    }
    return std::nullopt;
}

std::vector<Payment> PaymentRepository::findByMemberId(int memberId) {
    std::vector<Payment> list;
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT p.*, (m.first_name || ' ' || m.last_name) AS member_name "
        "FROM payments p JOIN members m ON p.member_id = m.member_id "
        "WHERE p.member_id = ? ORDER BY p.payment_id DESC;"
    );
    query.addBindValue(memberId);
    if (query.exec()) {
        while (query.next()) list.push_back(mapPayment(query));
    }
    return list;
}

std::vector<Payment> PaymentRepository::findAllPaged(int limit, int offset) {
    std::vector<Payment> list;
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT p.*, (m.first_name || ' ' || m.last_name) AS member_name "
        "FROM payments p JOIN members m ON p.member_id = m.member_id "
        "ORDER BY p.payment_id DESC LIMIT ? OFFSET ?;"
    );
    query.addBindValue(limit);
    query.addBindValue(offset);
    if (query.exec()) {
        while (query.next()) list.push_back(mapPayment(query));
    }
    return list;
}

std::vector<Payment> PaymentRepository::findByDateRange(const QString& startDate, const QString& endDate) {
    std::vector<Payment> list;
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT p.*, (m.first_name || ' ' || m.last_name) AS member_name "
        "FROM payments p JOIN members m ON p.member_id = m.member_id "
        "WHERE p.payment_date >= ? AND p.payment_date <= ? ORDER BY p.payment_date DESC;"
    );
    query.addBindValue(startDate);
    query.addBindValue(endDate);
    if (query.exec()) {
        while (query.next()) list.push_back(mapPayment(query));
    }
    return list;
}

bool PaymentRepository::create(Payment& payment) {
    if (payment.getReceiptNumber().isEmpty()) {
        payment.setReceiptNumber(generateNextReceiptNumber());
    }

    QSqlQuery query = DatabaseManager::instance().prepare(
        "INSERT INTO payments (receipt_number, member_id, membership_id, amount, discount, final_amount, payment_method, payment_date, received_by, notes) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"
    );
    query.addBindValue(payment.getReceiptNumber());
    query.addBindValue(payment.getMemberId());
    query.addBindValue(payment.getMembershipId() > 0 ? QVariant(payment.getMembershipId()) : QVariant(QVariant::Int));
    query.addBindValue(payment.getAmount());
    query.addBindValue(payment.getDiscount());
    query.addBindValue(payment.getFinalAmount());
    query.addBindValue(payment.getPaymentMethod());
    query.addBindValue(payment.getPaymentDate().isEmpty() ? QDate::currentDate().toString("yyyy-MM-dd") : payment.getPaymentDate());
    query.addBindValue(payment.getReceivedBy());
    query.addBindValue(payment.getNotes());

    if (query.exec()) {
        payment.setId(query.lastInsertId().toInt());
        return true;
    }
    qWarning() << "PaymentRepository::create error:" << query.lastError().text();
    return false;
}

QString PaymentRepository::generateNextReceiptNumber() {
    QSqlQuery query("SELECT MAX(payment_id) FROM payments;", DatabaseManager::instance().getDatabase());
    int maxId = 0;
    if (query.next()) maxId = query.value(0).toInt();
    return QString("REC-%1").arg(maxId + 1, 6, 10, QChar('0'));
}

double PaymentRepository::calculateMemberTotalPayments(int memberId) {
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT SUM(final_amount) FROM payments WHERE member_id = ?;"
    );
    query.addBindValue(memberId);
    if (query.exec() && query.next()) {
        return query.value(0).toDouble();
    }
    return 0.0;
}

double PaymentRepository::getTodayRevenue() {
    QString today = QDate::currentDate().toString("yyyy-MM-dd");
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT SUM(final_amount) FROM payments WHERE payment_date = ?;"
    );
    query.addBindValue(today);
    if (query.exec() && query.next()) {
        return query.value(0).toDouble();
    }
    return 0.0;
}

double PaymentRepository::getMonthlyRevenue() {
    QString monthPrefix = QDate::currentDate().toString("yyyy-MM-");
    QSqlQuery query = DatabaseManager::instance().prepare(
        "SELECT SUM(final_amount) FROM payments WHERE payment_date LIKE ?;"
    );
    query.addBindValue(monthPrefix + "%");
    if (query.exec() && query.next()) {
        return query.value(0).toDouble();
    }
    return 0.0;
}

double PaymentRepository::getTotalRevenue() {
    QSqlQuery query("SELECT SUM(final_amount) FROM payments;", DatabaseManager::instance().getDatabase());
    if (query.next()) {
        return query.value(0).toDouble();
    }
    return 0.0;
}

} // namespace FitCore
