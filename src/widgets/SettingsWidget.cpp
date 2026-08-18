#include "widgets/SettingsWidget.h"
#include "utils/ThemeManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QMessageBox>

namespace FitCore {

SettingsWidget::SettingsWidget(QWidget *parent)
    : QWidget(parent) {
    setupUi();
    refreshData();
}

void SettingsWidget::setupUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(20);

    QLabel *title = new QLabel("System Configuration & Database Terminal", this);
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #0F172A;");
    mainLayout->addWidget(title);

    // Box 1: Gym Profile Parameters
    QGroupBox *profileBox = new QGroupBox("Gym Profile Parameters", this);
    QFormLayout *pForm = new QFormLayout(profileBox);

    m_gymNameInput = new QLineEdit(this);
    m_phoneInput = new QLineEdit(this);
    m_emailInput = new QLineEdit(this);
    m_addressInput = new QLineEdit(this);
    m_currencyInput = new QLineEdit(this);
    m_taxRateInput = new QLineEdit(this);

    pForm->addRow("Gym Name:", m_gymNameInput);
    pForm->addRow("Contact Phone:", m_phoneInput);
    pForm->addRow("Support Email:", m_emailInput);
    pForm->addRow("Gym Address:", m_addressInput);
    pForm->addRow("Currency Symbol:", m_currencyInput);
    pForm->addRow("Sales Tax Rate (%):", m_taxRateInput);

    QPushButton *saveBtn = new QPushButton("Save System Settings", this);
    saveBtn->setObjectName("successBtn");
    pForm->addRow("", saveBtn);
    mainLayout->addWidget(profileBox);

    // Box 2: Database Hot Backup & Restore Terminal
    QGroupBox *dbBox = new QGroupBox("Database Maintenance & Hot Backup Terminal", this);
    QVBoxLayout *dbLayout = new QVBoxLayout(dbBox);

    QLabel *dbDesc = new QLabel("FitCore supports live hot SQLite backups ('VACUUM INTO') without locking database connections.", dbBox);
    dbDesc->setStyleSheet("color: #64748B; font-size: 12px;");
    dbLayout->addWidget(dbDesc);

    QHBoxLayout *dbBtnLayout = new QHBoxLayout();
    QPushButton *backupBtn = new QPushButton("Create Hot Database Backup", this);
    QPushButton *restoreBtn = new QPushButton("Restore Database from File", this);
    restoreBtn->setObjectName("dangerBtn");

    dbBtnLayout->addWidget(backupBtn);
    dbBtnLayout->addWidget(restoreBtn);
    dbLayout->addLayout(dbBtnLayout);

    m_backupStatusLbl = new QLabel("", dbBox);
    m_backupStatusLbl->setStyleSheet("font-weight: bold; color: #10B981; margin-top: 6px;");
    dbLayout->addWidget(m_backupStatusLbl);

    mainLayout->addWidget(dbBox);
    mainLayout->addStretch();

    connect(saveBtn, &QPushButton::clicked, this, &SettingsWidget::onSaveSettingsClicked);
    connect(backupBtn, &QPushButton::clicked, this, &SettingsWidget::onCreateBackupClicked);
    connect(restoreBtn, &QPushButton::clicked, this, &SettingsWidget::onRestoreBackupClicked);
}

void SettingsWidget::refreshData() {
    m_gymNameInput->setText(m_settingsRepo.get("gym_name", "FitCore Fitness Center"));
    m_phoneInput->setText(m_settingsRepo.get("gym_phone", "+92 300 1234567"));
    m_emailInput->setText(m_settingsRepo.get("gym_email", "info@fitcoregym.com"));
    m_addressInput->setText(m_settingsRepo.get("gym_address", "Main Boulevard, Blue Area, Islamabad"));
    m_currencyInput->setText(m_settingsRepo.get("currency_symbol", "PKR"));
    m_taxRateInput->setText(m_settingsRepo.get("tax_rate", "0.0"));
}

void SettingsWidget::onSaveSettingsClicked() {
    m_settingsRepo.set("gym_name", m_gymNameInput->text().trimmed());
    m_settingsRepo.set("gym_phone", m_phoneInput->text().trimmed());
    m_settingsRepo.set("gym_email", m_emailInput->text().trimmed());
    m_settingsRepo.set("gym_address", m_addressInput->text().trimmed());
    m_settingsRepo.set("currency_symbol", m_currencyInput->text().trimmed());
    m_settingsRepo.set("tax_rate", m_taxRateInput->text().trimmed());

    QMessageBox::information(this, "Settings Saved", "System configuration saved successfully.");
}

void SettingsWidget::onCreateBackupClicked() {
    BackupResult res = m_backupService.createBackup("backups");
    if (res.success) {
        m_backupStatusLbl->setStyleSheet("color: #10B981; font-weight: bold;");
        m_backupStatusLbl->setText(res.message);
        QMessageBox::information(this, "Backup Created", res.message);
    } else {
        m_backupStatusLbl->setStyleSheet("color: #EF4444; font-weight: bold;");
        m_backupStatusLbl->setText(res.message);
        QMessageBox::critical(this, "Backup Error", res.message);
    }
}

void SettingsWidget::onRestoreBackupClicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Select Backup File to Restore", "backups", "Database Files (*.db)");
    if (fileName.isEmpty()) return;

    QMessageBox::StandardButton reply = QMessageBox::warning(
        this, "Restore Database Confirmation",
        "Restoring a database backup will overwrite all current system data.\nAn emergency backup of the current state will be created automatically before restoring.\n\nDo you wish to proceed?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        BackupResult res = m_backupService.restoreBackup(fileName);
        if (res.success) {
            QMessageBox::information(this, "Restore Complete", res.message + "\nPlease restart application to apply changes.");
        } else {
            QMessageBox::critical(this, "Restore Failed", res.message);
        }
    }
}

} // namespace FitCore
