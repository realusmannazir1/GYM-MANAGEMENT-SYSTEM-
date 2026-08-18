#include "widgets/EquipmentWidget.h"
#include "utils/ThemeManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QTabWidget>
#include <QHeaderView>
#include <QDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QLabel>

namespace FitCore {

EquipmentWidget::EquipmentWidget(QWidget *parent)
    : QWidget(parent) {
    setupUi();
    refreshData();
}

void EquipmentWidget::setupUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    QHBoxLayout *topLayout = new QHBoxLayout();
    QLabel *title = new QLabel("Equipment Assets & Maintenance Inventory", this);
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #0F172A;");

    QPushButton *addBtn = new QPushButton("+ Add New Equipment", this);
    addBtn->setObjectName("successBtn");

    topLayout->addWidget(title);
    topLayout->addStretch();
    topLayout->addWidget(addBtn);
    mainLayout->addLayout(topLayout);

    QTabWidget *tabs = new QTabWidget(this);

    // Equipment Assets
    QWidget *eqTab = new QWidget();
    QVBoxLayout *eqLayout = new QVBoxLayout(eqTab);
    m_equipmentTable = new QTableWidget(eqTab);
    m_equipmentTable->setColumnCount(7);
    m_equipmentTable->setHorizontalHeaderLabels({"Code", "Name", "Category", "Brand", "Condition", "Status", "Action"});
    m_equipmentTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_equipmentTable->verticalHeader()->setDefaultSectionSize(42);
    m_equipmentTable->verticalHeader()->setVisible(false);
    eqLayout->addWidget(m_equipmentTable);
    tabs->addTab(eqTab, "Equipment Assets");

    // Maintenance Due & History
    QWidget *mTab = new QWidget();
    QVBoxLayout *mLayout = new QVBoxLayout(mTab);
    m_maintenanceTable = new QTableWidget(mTab);
    m_maintenanceTable->setColumnCount(7);
    m_maintenanceTable->setHorizontalHeaderLabels({"Equipment Code", "Equipment Name", "Service Date", "Problem", "Technician", "Cost", "Status"});
    m_maintenanceTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_maintenanceTable->verticalHeader()->setDefaultSectionSize(42);
    m_maintenanceTable->verticalHeader()->setVisible(false);
    mLayout->addWidget(m_maintenanceTable);
    tabs->addTab(mTab, "Maintenance Due & History");

    mainLayout->addWidget(tabs);

    connect(addBtn, &QPushButton::clicked, this, &EquipmentWidget::onAddEquipmentClicked);
}

void EquipmentWidget::refreshData() {
    loadEquipmentTable();
    loadMaintenanceTable();
}

void EquipmentWidget::loadEquipmentTable() {
    auto list = m_equipmentRepo.findAll();
    m_equipmentTable->setRowCount(0);
    int r = 0;
    for (const auto& e : list) {
        m_equipmentTable->insertRow(r);
        m_equipmentTable->setItem(r, 0, new QTableWidgetItem(e.getEquipmentCode()));
        m_equipmentTable->setItem(r, 1, new QTableWidgetItem(e.getEquipmentName()));
        m_equipmentTable->setItem(r, 2, new QTableWidgetItem(e.getCategory()));
        m_equipmentTable->setItem(r, 3, new QTableWidgetItem(e.getBrand()));
        m_equipmentTable->setItem(r, 4, new QTableWidgetItem(e.getCondition()));

        QLabel *badge = new QLabel(e.getStatus(), this);
        badge->setStyleSheet(ThemeManager::getBadgeStyle(e.getStatus()));
        badge->setAlignment(Qt::AlignCenter);
        m_equipmentTable->setCellWidget(r, 5, badge);

        QPushButton *maintBtn = new QPushButton("🔧 Service Log", this);
        maintBtn->setObjectName("secondaryBtn");
        maintBtn->setToolTip("Log Equipment Maintenance Service");
        maintBtn->setStyleSheet("padding: 4px 10px; font-weight: 600;");
        int id = e.getId();
        connect(maintBtn, &QPushButton::clicked, this, [this, id]() { onLogMaintenanceClicked(id); });
        m_equipmentTable->setCellWidget(r, 6, maintBtn);
        r++;
    }
}

void EquipmentWidget::loadMaintenanceTable() {
    auto list = m_equipmentRepo.getUpcomingMaintenanceAlerts(60);
    m_maintenanceTable->setRowCount(0);
    int r = 0;
    for (const auto& m : list) {
        m_maintenanceTable->insertRow(r);
        m_maintenanceTable->setItem(r, 0, new QTableWidgetItem(m.equipmentCode));
        m_maintenanceTable->setItem(r, 1, new QTableWidgetItem(m.equipmentName));
        m_maintenanceTable->setItem(r, 2, new QTableWidgetItem(m.maintenanceDate));
        m_maintenanceTable->setItem(r, 3, new QTableWidgetItem(m.problemDescription));
        m_maintenanceTable->setItem(r, 4, new QTableWidgetItem(m.technicianName));
        m_maintenanceTable->setItem(r, 5, new QTableWidgetItem(QString("Rs. %1").arg(m.cost, 0, 'f', 0)));

        QLabel *badge = new QLabel(m.status, this);
        badge->setStyleSheet(ThemeManager::getBadgeStyle(m.status));
        badge->setAlignment(Qt::AlignCenter);
        m_maintenanceTable->setCellWidget(r, 6, badge);
        r++;
    }
}

void EquipmentWidget::onAddEquipmentClicked() {
    QDialog dlg(this);
    dlg.setWindowTitle("Add Gym Equipment");
    dlg.resize(480, 460);
    dlg.setMinimumSize(380, 360);
    dlg.setStyleSheet(ThemeManager::getLightThemeQss());
    dlg.setWindowFlags(Qt::Window | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);

    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    QFormLayout *form = new QFormLayout();

    QLineEdit *codeInput = new QLineEdit(&dlg);
    codeInput->setText(m_equipmentRepo.generateNextEquipmentCode());
    codeInput->setReadOnly(true);

    QLineEdit *nameInput = new QLineEdit(&dlg);
    QComboBox *catCombo = new QComboBox(&dlg);
    catCombo->addItems({"Cardio", "Strength", "Free Weights", "Functional", "Other"});

    QLineEdit *brandInput = new QLineEdit(&dlg);
    QDoubleSpinBox *priceSpin = new QDoubleSpinBox(&dlg);
    priceSpin->setRange(0.0, 5000000.0);
    priceSpin->setPrefix("Rs. ");

    QComboBox *condCombo = new QComboBox(&dlg);
    condCombo->addItems({"Excellent", "Good", "Fair", "Needs Maintenance", "Damaged"});

    form->addRow("Equipment Code:", codeInput);
    form->addRow("Equipment Name *:", nameInput);
    form->addRow("Category *:", catCombo);
    form->addRow("Brand:", brandInput);
    form->addRow("Purchase Price:", priceSpin);
    form->addRow("Condition:", condCombo);

    layout->addLayout(form);

    QHBoxLayout *btnBox = new QHBoxLayout();
    QPushButton *cancelBtn = new QPushButton("Cancel", &dlg);
    cancelBtn->setObjectName("secondaryBtn");
    QPushButton *saveBtn = new QPushButton("Save Equipment", &dlg);
    saveBtn->setObjectName("successBtn");

    btnBox->addWidget(cancelBtn);
    btnBox->addWidget(saveBtn);
    layout->addLayout(btnBox);

    connect(cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, [&]() {
        if (nameInput->text().trimmed().isEmpty()) return;

        Equipment e;
        e.setEquipmentCode(codeInput->text());
        e.setEquipmentName(nameInput->text().trimmed());
        e.setCategory(catCombo->currentText());
        e.setBrand(brandInput->text().trimmed());
        e.setPurchasePrice(priceSpin->value());
        e.setCondition(condCombo->currentText());
        e.setStatus("Available");
        e.setPurchaseDate(QDate::currentDate().toString("yyyy-MM-dd"));

        if (m_equipmentRepo.create(e)) {
            dlg.accept();
            refreshData();
        }
    });

    dlg.exec();
}

void EquipmentWidget::onLogMaintenanceClicked(int equipmentId) {
    QDialog dlg(this);
    dlg.setWindowTitle("Log Maintenance Service");
    dlg.resize(480, 400);
    dlg.setMinimumSize(380, 320);
    dlg.setStyleSheet(ThemeManager::getLightThemeQss());
    dlg.setWindowFlags(Qt::Window | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);

    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    QFormLayout *form = new QFormLayout();

    QLineEdit *probInput = new QLineEdit(&dlg);
    probInput->setText("Routine Servicing & Cable Inspection");

    QLineEdit *servInput = new QLineEdit(&dlg);
    servInput->setText("Lubricated pulleys and tightened cables.");

    QDoubleSpinBox *costSpin = new QDoubleSpinBox(&dlg);
    costSpin->setRange(0.0, 500000.0);
    costSpin->setValue(2500.0);
    costSpin->setPrefix("Rs. ");

    QLineEdit *techInput = new QLineEdit(&dlg);
    techInput->setText("ProGym Services");

    QComboBox *statusCombo = new QComboBox(&dlg);
    statusCombo->addItems({"Completed", "In Progress", "Pending"});

    form->addRow("Problem / Reason:", probInput);
    form->addRow("Service Performed:", servInput);
    form->addRow("Service Cost (PKR):", costSpin);
    form->addRow("Technician / Vendor:", techInput);
    form->addRow("Maintenance Status:", statusCombo);

    layout->addLayout(form);

    QHBoxLayout *btnBox = new QHBoxLayout();
    QPushButton *cancelBtn = new QPushButton("Cancel", &dlg);
    cancelBtn->setObjectName("secondaryBtn");
    QPushButton *saveBtn = new QPushButton("Log Maintenance", &dlg);
    saveBtn->setObjectName("successBtn");

    btnBox->addWidget(cancelBtn);
    btnBox->addWidget(saveBtn);
    layout->addLayout(btnBox);

    connect(cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, [&]() {
        MaintenanceLogItem item;
        item.equipmentId = equipmentId;
        item.maintenanceDate = QDate::currentDate().toString("yyyy-MM-dd");
        item.problemDescription = probInput->text().trimmed();
        item.serviceDescription = servInput->text().trimmed();
        item.cost = costSpin->value();
        item.technicianName = techInput->text().trimmed();
        item.nextMaintenanceDate = QDate::currentDate().addDays(90).toString("yyyy-MM-dd");
        item.status = statusCombo->currentText();

        if (m_equipmentRepo.addMaintenanceRecord(item)) {
            QMessageBox::information(&dlg, "Service Logged", "Equipment maintenance record saved.");
            dlg.accept();
            refreshData();
        }
    });

    dlg.exec();
}

} // namespace FitCore
