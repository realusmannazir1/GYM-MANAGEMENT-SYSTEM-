#include "widgets/ProgressWidget.h"
#include "utils/ThemeManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QDialog>
#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QLabel>

namespace FitCore {

ProgressWidget::ProgressWidget(QWidget *parent)
    : QWidget(parent) {
    setupUi();
    refreshData();
}

void ProgressWidget::setupUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    QHBoxLayout *topLayout = new QHBoxLayout();
    QLabel *title = new QLabel("Body Composition & Progress Tracking", this);
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #F8FAFC;");

    QPushButton *addRecordBtn = new QPushButton("+ Log Measurements", this);
    addRecordBtn->setObjectName("successBtn");

    topLayout->addWidget(title);
    topLayout->addStretch();
    topLayout->addWidget(addRecordBtn);
    mainLayout->addLayout(topLayout);

    QHBoxLayout *filterLayout = new QHBoxLayout();
    m_memberFilter = new QComboBox(this);
    m_memberFilter->addItem("All Members", 0);

    filterLayout->addWidget(new QLabel("Filter by Member:", this));
    filterLayout->addWidget(m_memberFilter, 2);
    filterLayout->addStretch(3);
    mainLayout->addLayout(filterLayout);

    m_table = new QTableWidget(this);
    m_table->setColumnCount(9);
    m_table->setHorizontalHeaderLabels({"Member", "Date", "Weight (kg)", "Height (cm)", "BMI", "Body Fat %", "Waist (cm)", "Chest (cm)", "Arms (cm)"});
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mainLayout->addWidget(m_table);

    connect(addRecordBtn, &QPushButton::clicked, this, &ProgressWidget::onAddRecordClicked);
    connect(m_memberFilter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]() { loadProgressTable(); });
}

void ProgressWidget::refreshData() {
    auto members = m_memberRepo.findAll();
    m_memberFilter->clear();
    m_memberFilter->addItem("All Members", 0);
    for (const auto& m : members) {
        m_memberFilter->addItem(m.getFullName() + " (" + m.getMembershipNumber() + ")", m.getId());
    }
    loadProgressTable();
}

void ProgressWidget::loadProgressTable() {
    int selectedMemberId = m_memberFilter->currentData().toInt();
    std::vector<ProgressRecord> records;

    if (selectedMemberId > 0) {
        records = m_progressRepo.findByMemberId(selectedMemberId);
    } else {
        auto members = m_memberRepo.findAll();
        for (const auto& m : members) {
            auto sub = m_progressRepo.findByMemberId(m.getId());
            records.insert(records.end(), sub.begin(), sub.end());
        }
    }

    m_table->setRowCount(0);
    int r = 0;
    for (const auto& pr : records) {
        m_table->insertRow(r);
        m_table->setItem(r, 0, new QTableWidgetItem(pr.getMemberName()));
        m_table->setItem(r, 1, new QTableWidgetItem(pr.getRecordDate()));
        m_table->setItem(r, 2, new QTableWidgetItem(QString::number(pr.getWeightKg(), 'f', 1)));
        m_table->setItem(r, 3, new QTableWidgetItem(QString::number(pr.getHeightCm(), 'f', 1)));
        m_table->setItem(r, 4, new QTableWidgetItem(QString("%1 (%2)").arg(pr.getBmi(), 0, 'f', 1).arg(pr.getBmiCategory())));
        m_table->setItem(r, 5, new QTableWidgetItem(QString::number(pr.getBodyFatPercentage(), 'f', 1)));
        m_table->setItem(r, 6, new QTableWidgetItem(QString::number(pr.getWaistCm(), 'f', 1)));
        m_table->setItem(r, 7, new QTableWidgetItem(QString::number(pr.getChestCm(), 'f', 1)));
        m_table->setItem(r, 8, new QTableWidgetItem(QString::number(pr.getArmsCm(), 'f', 1)));
        r++;
    }
}

void ProgressWidget::onAddRecordClicked() {
    QDialog dlg(this);
    dlg.setWindowTitle("Log Body Progress & Measurements");
    dlg.setFixedSize(480, 500);
    dlg.setStyleSheet(ThemeManager::getDarkThemeQss());

    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    QFormLayout *form = new QFormLayout();

    QComboBox *memberCombo = new QComboBox(&dlg);
    auto members = m_memberRepo.findAll();
    for (const auto& m : members) {
        memberCombo->addItem(m.getFullName() + " (" + m.getMembershipNumber() + ")", m.getId());
    }

    QDateEdit *dateEdit = new QDateEdit(&dlg);
    dateEdit->setDisplayFormat("yyyy-MM-dd");
    dateEdit->setDate(QDate::currentDate());
    dateEdit->setCalendarPopup(true);

    QDoubleSpinBox *weightSpin = new QDoubleSpinBox(&dlg);
    weightSpin->setRange(20.0, 300.0);
    weightSpin->setValue(75.0);
    weightSpin->setSuffix(" kg");

    QDoubleSpinBox *heightSpin = new QDoubleSpinBox(&dlg);
    heightSpin->setRange(50.0, 250.0);
    heightSpin->setValue(175.0);
    heightSpin->setSuffix(" cm");

    QLabel *bmiPreviewLbl = new QLabel("Calculated BMI: 24.5 (Normal Weight)", &dlg);
    bmiPreviewLbl->setStyleSheet("font-weight: bold; color: #3B82F6;");

    auto updateBmiPreview = [=]() {
        double w = weightSpin->value();
        double h = heightSpin->value() / 100.0;
        if (h > 0) {
            double bmi = w / (h * h);
            QString cat = "Normal";
            if (bmi < 18.5) cat = "Underweight";
            else if (bmi >= 25.0 && bmi < 29.9) cat = "Overweight";
            else if (bmi >= 30.0) cat = "Obese";
            bmiPreviewLbl->setText(QString("Calculated BMI: %1 (%2)").arg(bmi, 0, 'f', 1).arg(cat));
        }
    };

    connect(weightSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), &dlg, updateBmiPreview);
    connect(heightSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), &dlg, updateBmiPreview);

    QDoubleSpinBox *fatSpin = new QDoubleSpinBox(&dlg);
    fatSpin->setRange(0.0, 60.0);
    fatSpin->setValue(18.0);
    fatSpin->setSuffix(" %");

    QDoubleSpinBox *waistSpin = new QDoubleSpinBox(&dlg);
    waistSpin->setRange(20.0, 200.0);
    waistSpin->setValue(82.0);
    waistSpin->setSuffix(" cm");

    QDoubleSpinBox *chestSpin = new QDoubleSpinBox(&dlg);
    chestSpin->setRange(20.0, 200.0);
    chestSpin->setValue(98.0);
    chestSpin->setSuffix(" cm");

    QDoubleSpinBox *armsSpin = new QDoubleSpinBox(&dlg);
    armsSpin->setRange(10.0, 100.0);
    armsSpin->setValue(35.0);
    armsSpin->setSuffix(" cm");

    form->addRow("Select Member *:", memberCombo);
    form->addRow("Record Date *:", dateEdit);
    form->addRow("Weight (kg) *:", weightSpin);
    form->addRow("Height (cm) *:", heightSpin);
    form->addRow("", bmiPreviewLbl);
    form->addRow("Body Fat %:", fatSpin);
    form->addRow("Waist (cm):", waistSpin);
    form->addRow("Chest (cm):", chestSpin);
    form->addRow("Arms (cm):", armsSpin);

    layout->addLayout(form);

    QHBoxLayout *btnBox = new QHBoxLayout();
    QPushButton *cancelBtn = new QPushButton("Cancel", &dlg);
    cancelBtn->setObjectName("secondaryBtn");
    QPushButton *saveBtn = new QPushButton("Save Progress Record", &dlg);
    saveBtn->setObjectName("successBtn");

    btnBox->addWidget(cancelBtn);
    btnBox->addWidget(saveBtn);
    layout->addLayout(btnBox);

    connect(cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, [&]() {
        if (memberCombo->currentIndex() < 0) return;

        ProgressRecord pr;
        pr.setMemberId(memberCombo->currentData().toInt());
        pr.setRecordDate(dateEdit->date().toString("yyyy-MM-dd"));
        pr.setWeightKg(weightSpin->value());
        pr.setHeightCm(heightSpin->value());
        pr.setBodyFatPercentage(fatSpin->value());
        pr.setWaistCm(waistSpin->value());
        pr.setChestCm(chestSpin->value());
        pr.setArmsCm(armsSpin->value());
        pr.setBmi(pr.calculateBmi());

        if (m_progressRepo.create(pr)) {
            QMessageBox::information(&dlg, "Record Saved", "Body progress entry recorded successfully.");
            dlg.accept();
            refreshData();
        }
    });

    dlg.exec();
}

} // namespace FitCore
