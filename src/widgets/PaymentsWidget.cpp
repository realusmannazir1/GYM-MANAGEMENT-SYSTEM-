#include "widgets/PaymentsWidget.h"
#include "utils/ThemeManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QLabel>

namespace FitCore {

PaymentsWidget::PaymentsWidget(QWidget *parent)
    : QWidget(parent) {
    setupUi();
    refreshData();
}

void PaymentsWidget::setupUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    QHBoxLayout *topLayout = new QHBoxLayout();
    QLabel *title = new QLabel("Payments & Financial Transactions", this);
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #0F172A;");

    QPushButton *recordBtn = new QPushButton("+ Record Manual Payment", this);
    recordBtn->setObjectName("successBtn");

    topLayout->addWidget(title);
    topLayout->addStretch();
    topLayout->addWidget(recordBtn);
    mainLayout->addLayout(topLayout);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    m_searchInput = new QLineEdit(this);
    m_searchInput->setPlaceholderText("Search by receipt number (REC-XXXXXX) or member name...");

    searchLayout->addWidget(m_searchInput);
    mainLayout->addLayout(searchLayout);

    m_table = new QTableWidget(this);
    m_table->setColumnCount(7);
    m_table->setHorizontalHeaderLabels({"Receipt #", "Member Name", "Amount", "Discount", "Final Paid", "Method", "Date"});
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mainLayout->addWidget(m_table);

    connect(recordBtn, &QPushButton::clicked, this, &PaymentsWidget::onRecordPaymentClicked);
    connect(m_searchInput, &QLineEdit::textChanged, this, [this]() { loadPaymentsTable(); });
}

void PaymentsWidget::refreshData() {
    loadPaymentsTable();
}

void PaymentsWidget::loadPaymentsTable() {
    QString query = m_searchInput->text().trimmed();
    std::vector<Payment> payments;

    if (query.startsWith("REC-", Qt::CaseInsensitive)) {
        auto opt = m_paymentRepo.findByReceiptNumber(query);
        if (opt.has_value()) payments.push_back(opt.value());
    } else {
        payments = m_paymentRepo.findAllPaged(100, 0);
    }

    m_table->setRowCount(0);
    int r = 0;
    for (const auto& p : payments) {
        m_table->insertRow(r);
        m_table->setItem(r, 0, new QTableWidgetItem(p.getReceiptNumber()));
        m_table->setItem(r, 1, new QTableWidgetItem(p.getMemberName()));
        m_table->setItem(r, 2, new QTableWidgetItem(QString("Rs. %1").arg(p.getAmount(), 0, 'f', 0)));
        m_table->setItem(r, 3, new QTableWidgetItem(QString("Rs. %1").arg(p.getDiscount(), 0, 'f', 0)));
        m_table->setItem(r, 4, new QTableWidgetItem(QString("Rs. %1").arg(p.getFinalAmount(), 0, 'f', 0)));
        m_table->setItem(r, 5, new QTableWidgetItem(p.getPaymentMethod()));
        m_table->setItem(r, 6, new QTableWidgetItem(p.getPaymentDate()));
        r++;
    }
}

void PaymentsWidget::onRecordPaymentClicked() {
    QDialog dlg(this);
    dlg.setWindowTitle("Record Payment Receipt");
    dlg.setFixedSize(450, 420);
    dlg.setStyleSheet(ThemeManager::getDarkThemeQss());

    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    QFormLayout *form = new QFormLayout();

    QLineEdit *recNumInput = new QLineEdit(&dlg);
    recNumInput->setText(m_paymentRepo.generateNextReceiptNumber());
    recNumInput->setReadOnly(true);

    QComboBox *memberCombo = new QComboBox(&dlg);
    auto members = m_memberRepo.findAll();
    for (const auto& m : members) {
        memberCombo->addItem(m.getFullName() + " (" + m.getMembershipNumber() + ")", m.getId());
    }

    QDoubleSpinBox *amountSpin = new QDoubleSpinBox(&dlg);
    amountSpin->setRange(1.0, 500000.0);
    amountSpin->setValue(3500.0);
    amountSpin->setPrefix("Rs. ");

    QDoubleSpinBox *discSpin = new QDoubleSpinBox(&dlg);
    discSpin->setRange(0.0, 50000.0);
    discSpin->setPrefix("Rs. ");

    QComboBox *methodCombo = new QComboBox(&dlg);
    methodCombo->addItems({"Cash", "Card", "Bank Transfer", "Other"});

    QLineEdit *notesInput = new QLineEdit(&dlg);

    form->addRow("Receipt #:", recNumInput);
    form->addRow("Select Member *:", memberCombo);
    form->addRow("Gross Amount *:", amountSpin);
    form->addRow("Discount:", discSpin);
    form->addRow("Payment Method *:", methodCombo);
    form->addRow("Payment Notes:", notesInput);

    layout->addLayout(form);

    QHBoxLayout *btnBox = new QHBoxLayout();
    QPushButton *cancelBtn = new QPushButton("Cancel", &dlg);
    cancelBtn->setObjectName("secondaryBtn");
    QPushButton *saveBtn = new QPushButton("Generate Receipt", &dlg);
    saveBtn->setObjectName("successBtn");

    btnBox->addWidget(cancelBtn);
    btnBox->addWidget(saveBtn);
    layout->addLayout(btnBox);

    connect(cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, [&]() {
        if (memberCombo->currentIndex() < 0) return;

        Payment p;
        p.setReceiptNumber(recNumInput->text());
        p.setMemberId(memberCombo->currentData().toInt());
        p.setAmount(amountSpin->value());
        p.setDiscount(discSpin->value());
        p.setFinalAmount(amountSpin->value() - discSpin->value());
        p.setPaymentMethod(methodCombo->currentText());
        p.setReceivedBy(1);
        p.setNotes(notesInput->text().trimmed());

        if (m_paymentRepo.create(p)) {
            QMessageBox::information(&dlg, "Receipt Issued", "Receipt " + p.getReceiptNumber() + " created cleanly.");
            dlg.accept();
            refreshData();
        }
    });

    dlg.exec();
}

} // namespace FitCore
