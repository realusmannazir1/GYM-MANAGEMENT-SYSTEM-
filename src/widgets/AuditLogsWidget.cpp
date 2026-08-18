#include "widgets/AuditLogsWidget.h"
#include "utils/ThemeManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>

namespace FitCore {

AuditLogsWidget::AuditLogsWidget(QWidget *parent)
    : QWidget(parent) {
    setupUi();
    refreshData();
}

void AuditLogsWidget::setupUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    QHBoxLayout *topLayout = new QHBoxLayout();
    QLabel *title = new QLabel("System Audit Trail & Security Logs", this);
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #0F172A;");

    topLayout->addWidget(title);
    topLayout->addStretch();
    mainLayout->addLayout(topLayout);

    m_table = new QTableWidget(this);
    m_table->setColumnCount(6);
    m_table->setHorizontalHeaderLabels({"Audit ID", "User", "Action", "Target Entity", "Entity ID", "Description / Timestamp"});
    m_table->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    m_table->verticalHeader()->setDefaultSectionSize(42);
    m_table->verticalHeader()->setVisible(false);
    mainLayout->addWidget(m_table);
}

void AuditLogsWidget::refreshData() {
    loadAuditTable();
}

void AuditLogsWidget::loadAuditTable() {
    auto list = m_auditRepo.findAllPaged(200, 0);
    m_table->setRowCount(0);
    int r = 0;
    for (const auto& a : list) {
        m_table->insertRow(r);
        m_table->setItem(r, 0, new QTableWidgetItem(QString::number(a.getId())));
        m_table->setItem(r, 1, new QTableWidgetItem(a.getUsername().isEmpty() ? "System" : a.getUsername()));
        m_table->setItem(r, 2, new QTableWidgetItem(a.getAction()));
        m_table->setItem(r, 3, new QTableWidgetItem(a.getEntityName()));
        m_table->setItem(r, 4, new QTableWidgetItem(a.getEntityId() > 0 ? QString::number(a.getEntityId()) : "-"));
        m_table->setItem(r, 5, new QTableWidgetItem(QString("[%1] %2").arg(a.getTimestamp(), a.getDescription())));
        r++;
    }
}

} // namespace FitCore
