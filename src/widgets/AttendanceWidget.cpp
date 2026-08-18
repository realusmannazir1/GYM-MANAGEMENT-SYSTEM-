#include "widgets/AttendanceWidget.h"
#include "utils/ThemeManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QDate>
#include <QMessageBox>

namespace FitCore {

AttendanceWidget::AttendanceWidget(QWidget *parent)
    : QWidget(parent) {
    setupUi();
    refreshData();
}

void AttendanceWidget::setupUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    // Section 1: Title & Stats Header
    QHBoxLayout *topLayout = new QHBoxLayout();
    QLabel *title = new QLabel("Attendance & Fast Check-In Terminal", this);
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #0F172A;");

    m_todayCountLbl = new QLabel("Today's Visits: 0", this);
    m_todayCountLbl->setStyleSheet("font-size: 14px; font-weight: bold; color: #1E40AF; background-color: #EFF6FF; border: 1px solid #BFDBFE; padding: 6px 14px; border-radius: 8px;");

    topLayout->addWidget(title);
    topLayout->addStretch();
    topLayout->addWidget(m_todayCountLbl);
    mainLayout->addLayout(topLayout);

    // Section 2: Fast Check-In Control Box
    QGroupBox *checkInBox = new QGroupBox("Fast Member Check-In Terminal", this);
    QVBoxLayout *boxLayout = new QVBoxLayout(checkInBox);

    QHBoxLayout *inputRow = new QHBoxLayout();
    m_checkInInput = new QLineEdit(this);
    m_checkInInput->setPlaceholderText("Scan barcode or enter Membership # (e.g. MEM-000001)...");
    m_checkInInput->setStyleSheet("font-size: 15px; padding: 10px;");

    m_checkInBtn = new QPushButton("CHECK IN MEMBER", this);
    m_checkInBtn->setObjectName("successBtn");
    m_checkInBtn->setStyleSheet("padding: 10px 20px; font-size: 14px; font-weight: bold;");

    inputRow->addWidget(m_checkInInput, 3);
    inputRow->addWidget(m_checkInBtn, 1);
    boxLayout->addLayout(inputRow);

    m_statusFeedbackLbl = new QLabel("", this);
    m_statusFeedbackLbl->setStyleSheet("font-size: 13px; font-weight: bold; padding-top: 4px;");
    boxLayout->addWidget(m_statusFeedbackLbl);

    mainLayout->addWidget(checkInBox);

    // Section 3: Today's Live Attendance Table
    QGroupBox *todayBox = new QGroupBox(QString("Today's Check-Ins (%1)").arg(QDate::currentDate().toString("dd-MMM-yyyy")), this);
    QVBoxLayout *tLayout = new QVBoxLayout(todayBox);

    m_todayTable = new QTableWidget(this);
    m_todayTable->setColumnCount(6);
    m_todayTable->setHorizontalHeaderLabels({"Membership #", "Member Name", "Check-In Time", "Check-Out Time", "Duration", "Action"});
    m_todayTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_todayTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    tLayout->addWidget(m_todayTable);

    mainLayout->addWidget(todayBox);

    connect(m_checkInBtn, &QPushButton::clicked, this, &AttendanceWidget::onFastCheckIn);
    connect(m_checkInInput, &QLineEdit::returnPressed, this, &AttendanceWidget::onFastCheckIn);
}

void AttendanceWidget::refreshData() {
    loadTodayAttendance();
}

void AttendanceWidget::loadTodayAttendance() {
    QString today = QDate::currentDate().toString("yyyy-MM-dd");
    auto list = m_attendanceRepo.findByDate(today);

    m_todayCountLbl->setText(QString("Today's Visits: %1").arg(list.size()));
    m_todayTable->setRowCount(0);
    int r = 0;

    for (const auto& a : list) {
        m_todayTable->insertRow(r);
        m_todayTable->setItem(r, 0, new QTableWidgetItem(a.getMembershipNumber()));
        m_todayTable->setItem(r, 1, new QTableWidgetItem(a.getMemberName()));
        m_todayTable->setItem(r, 2, new QTableWidgetItem(a.getCheckInTime()));
        m_todayTable->setItem(r, 3, new QTableWidgetItem(a.isCheckedOut() ? a.getCheckOutTime() : "Active Session"));

        QLabel *durLbl = new QLabel(a.getDurationFormatted(), this);
        if (a.isCheckedOut()) {
            durLbl->setStyleSheet("color: #94A3B8; font-weight: 500;");
        } else {
            durLbl->setStyleSheet("color: #10B981; font-weight: bold;");
        }
        durLbl->setAlignment(Qt::AlignCenter);
        m_todayTable->setCellWidget(r, 4, durLbl);

        if (!a.isCheckedOut()) {
            QPushButton *checkOutBtn = new QPushButton("Check Out", this);
            checkOutBtn->setObjectName("secondaryBtn");
            checkOutBtn->setStyleSheet("padding: 4px 10px; font-size: 11px;");
            int attId = a.getId();
            connect(checkOutBtn, &QPushButton::clicked, this, [this, attId]() { onCheckOutClicked(attId); });
            m_todayTable->setCellWidget(r, 5, checkOutBtn);
        } else {
            QLabel *doneLbl = new QLabel("Completed", this);
            doneLbl->setStyleSheet("color: #64748B; font-size: 11px;");
            doneLbl->setAlignment(Qt::AlignCenter);
            m_todayTable->setCellWidget(r, 5, doneLbl);
        }
        r++;
    }
}

void AttendanceWidget::onFastCheckIn() {
    QString num = m_checkInInput->text().trimmed();
    if (num.isEmpty()) {
        m_statusFeedbackLbl->setStyleSheet("color: #F59E0B; font-weight: bold;");
        m_statusFeedbackLbl->setText("Please enter or scan a membership number.");
        return;
    }

    CheckInResult result = m_attendanceService.checkInByMembershipNumber(num, 1);
    if (result.success) {
        m_statusFeedbackLbl->setStyleSheet("color: #10B981; font-weight: bold;");
        m_statusFeedbackLbl->setText(result.message);
        m_checkInInput->clear();
        loadTodayAttendance();
    } else {
        m_statusFeedbackLbl->setStyleSheet("color: #EF4444; font-weight: bold;");
        m_statusFeedbackLbl->setText(result.message);
    }
}

void AttendanceWidget::onCheckOutClicked(int attendanceId) {
    if (m_attendanceService.checkOut(attendanceId, 1)) {
        m_statusFeedbackLbl->setStyleSheet("color: #3B82F6; font-weight: bold;");
        m_statusFeedbackLbl->setText("Member checked out successfully.");
        loadTodayAttendance();
    }
}

} // namespace FitCore
