#include "widgets/NotificationsWidget.h"
#include "utils/ThemeManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>

namespace FitCore {

NotificationsWidget::NotificationsWidget(QWidget *parent)
    : QWidget(parent) {
    setupUi();
    refreshData();
}

void NotificationsWidget::setupUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    QHBoxLayout *topLayout = new QHBoxLayout();
    QLabel *title = new QLabel("System Notifications & Expiry Alerts", this);
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #0F172A;");

    QPushButton *markAllBtn = new QPushButton("Mark All as Read", this);
    markAllBtn->setObjectName("secondaryBtn");

    topLayout->addWidget(title);
    topLayout->addStretch();
    topLayout->addWidget(markAllBtn);
    mainLayout->addLayout(topLayout);

    m_table = new QTableWidget(this);
    m_table->setColumnCount(4);
    m_table->setHorizontalHeaderLabels({"Alert Type", "Message", "Timestamp", "Status"});
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    mainLayout->addWidget(m_table);

    connect(markAllBtn, &QPushButton::clicked, this, &NotificationsWidget::onMarkAllReadClicked);
}

void NotificationsWidget::refreshData() {
    m_notificationService.checkAndGenerateExpiriesAndAlerts();
    loadNotificationsTable();
}

void NotificationsWidget::loadNotificationsTable() {
    auto list = m_notificationService.getUserNotifications(1, false);
    m_table->setRowCount(0);
    int r = 0;
    for (const auto& n : list) {
        m_table->insertRow(r);
        m_table->setItem(r, 0, new QTableWidgetItem(n.getType()));
        m_table->setItem(r, 1, new QTableWidgetItem(n.getMessage()));
        m_table->setItem(r, 2, new QTableWidgetItem(n.getCreatedAt()));

        QLabel *badge = new QLabel(n.isRead() ? "Read" : "Unread", this);
        badge->setStyleSheet(ThemeManager::getBadgeStyle(n.isRead() ? "Completed" : "Pending"));
        badge->setAlignment(Qt::AlignCenter);
        m_table->setCellWidget(r, 3, badge);
        r++;
    }
}

void NotificationsWidget::onMarkAllReadClicked() {
    m_notificationService.markAllAsRead(1);
    loadNotificationsTable();
}

} // namespace FitCore
