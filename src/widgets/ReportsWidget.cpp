#include "widgets/ReportsWidget.h"
#include "utils/ThemeManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QLabel>

namespace FitCore {

ReportsWidget::ReportsWidget(QWidget *parent)
    : QWidget(parent) {
    setupUi();
    refreshData();
}

void ReportsWidget::setupUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    QHBoxLayout *topLayout = new QHBoxLayout();
    QLabel *title = new QLabel("Analytics & Financial Reports", this);
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #0F172A;");

    topLayout->addWidget(title);
    topLayout->addStretch();
    mainLayout->addLayout(topLayout);

    QHBoxLayout *ctrlLayout = new QHBoxLayout();
    m_reportTypeCombo = new QComboBox(this);
    m_reportTypeCombo->addItems({"Members", "Payments", "Attendance"});

    QPushButton *genBtn = new QPushButton("Generate Preview", this);
    QPushButton *exportBtn = new QPushButton("Export to CSV", this);
    exportBtn->setObjectName("successBtn");

    ctrlLayout->addWidget(new QLabel("Select Report Category:", this));
    ctrlLayout->addWidget(m_reportTypeCombo, 2);
    ctrlLayout->addWidget(genBtn);
    ctrlLayout->addWidget(exportBtn);
    ctrlLayout->addStretch(2);

    mainLayout->addLayout(ctrlLayout);

    m_previewText = new QTextEdit(this);
    m_previewText->setReadOnly(true);
    m_previewText->setStyleSheet("font-family: 'Consolas', 'Courier New', monospace; font-size: 12px; line-height: 1.4;");

    mainLayout->addWidget(m_previewText);

    connect(genBtn, &QPushButton::clicked, this, &ReportsWidget::onGenerateReport);
    connect(exportBtn, &QPushButton::clicked, this, &ReportsWidget::onExportCSV);
}

void ReportsWidget::refreshData() {
    onGenerateReport();
}

void ReportsWidget::onGenerateReport() {
    QString type = m_reportTypeCombo->currentText();
    QString csv = m_reportService.generateCSVReport(type);
    m_previewText->setText(csv);
}

void ReportsWidget::onExportCSV() {
    QString type = m_reportTypeCombo->currentText();
    QString csvData = m_previewText->toPlainText();
    if (csvData.isEmpty()) return;

    QString fileName = QFileDialog::getSaveFileName(this, "Export CSV Report", QString("FitCore_%1_Report.csv").arg(type), "CSV Files (*.csv)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << csvData;
            file.close();
            QMessageBox::information(this, "Export Success", "Report exported successfully to:\n" + fileName);
        } else {
            QMessageBox::critical(this, "Export Error", "Failed to write CSV file.");
        }
    }
}

} // namespace FitCore
