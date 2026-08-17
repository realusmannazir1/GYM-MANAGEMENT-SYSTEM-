#ifndef PAYMENTSWIDGET_H
#define PAYMENTSWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include "repositories/PaymentRepository.h"
#include "repositories/MemberRepository.h"

namespace FitCore {

class PaymentsWidget : public QWidget {
    Q_OBJECT
public:
    explicit PaymentsWidget(QWidget *parent = nullptr);

    void refreshData();

private slots:
    void onRecordPaymentClicked();

private:
    void setupUi();
    void loadPaymentsTable();

    QLineEdit *m_searchInput;
    QTableWidget *m_table;

    PaymentRepository m_paymentRepo;
    MemberRepository m_memberRepo;
};

} // namespace FitCore

#endif // PAYMENTSWIDGET_H
