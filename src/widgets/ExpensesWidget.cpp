#include "widgets/ExpensesWidget.h"
#include "utils/ThemeManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QLabel>

namespace FitCore {

ExpensesWidget::ExpensesWidget(QWidget *parent)
    : QWidget(parent) {
    setupUi();
    refreshData();
}

void ExpensesWidget::setupUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    QHBoxLayout *topLayout = new QHBoxLayout();
    QLabel *title = new QLabel("Expense Outflow Ledger", this);
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #0F172A;");

    QPushButton *addBtn = new QPushButton("+ Record Expense", this);
    addBtn->setObjectName("successBtn");

    topLayout->addWidget(title);
    topLayout->addStretch();
    topLayout->addWidget(addBtn);
    mainLayout->addLayout(topLayout);

    QHBoxLayout *filterLayout = new QHBoxLayout();
    m_categoryFilter = new QComboBox(this);
    m_categoryFilter->addItems({"All", "Rent", "Utilities", "Equipment Maintenance", "Salaries", "Supplements", "Marketing", "Other"});

    filterLayout->addWidget(new QLabel("Category Filter:", this));
    filterLayout->addWidget(m_categoryFilter, 2);
    filterLayout->addStretch(3);
    mainLayout->addLayout(filterLayout);

    m_table = new QTableWidget(this);
    m_table->setColumnCount(6);
    m_table->setHorizontalHeaderLabels({"Category", "Description", "Amount", "Expense Date", "Payment Method", "Action"});
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    mainLayout->addWidget(m_table);

    connect(addBtn, &QPushButton::clicked, this, &ExpensesWidget::onAddExpenseClicked);
    connect(m_categoryFilter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]() { loadExpensesTable(); });
}

void ExpensesWidget::refreshData() {
    loadExpensesTable();
}

void ExpensesWidget::loadExpensesTable() {
    QString cat = m_categoryFilter->currentText();
    auto list = m_expenseRepo.findAll(cat);

    m_table->setRowCount(0);
    int r = 0;
    for (const auto& e : list) {
        m_table->insertRow(r);
        m_table->setItem(r, 0, new QTableWidgetItem(e.getCategory()));
        m_table->setItem(r, 1, new QTableWidgetItem(e.getDescription()));
        m_table->setItem(r, 2, new QTableWidgetItem(QString("Rs. %1").arg(e.getAmount(), 0, 'f', 0)));
        m_table->setItem(r, 3, new QTableWidgetItem(e.getExpenseDate()));
        m_table->setItem(r, 4, new QTableWidgetItem(e.getPaymentMethod()));

        QPushButton *delBtn = new QPushButton("Delete", this);
        delBtn->setObjectName("dangerBtn");
        delBtn->setStyleSheet("padding: 4px 8px; font-size: 11px;");
        int id = e.getId();
        connect(delBtn, &QPushButton::clicked, this, [this, id]() {
            if (m_expenseRepo.remove(id)) loadExpensesTable();
        });
        m_table->setCellWidget(r, 5, delBtn);
        r++;
    }
}

void ExpensesWidget::onAddExpenseClicked() {
    QDialog dlg(this);
    dlg.setWindowTitle("Record Operational Expense");
    dlg.resize(480, 400);
    dlg.setMinimumSize(380, 320);
    dlg.setStyleSheet(ThemeManager::getLightThemeQss());
    dlg.setWindowFlags(Qt::Window | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);

    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    QFormLayout *form = new QFormLayout();

    QComboBox *catCombo = new QComboBox(&dlg);
    catCombo->addItems({"Rent", "Utilities", "Equipment Maintenance", "Salaries", "Supplements", "Marketing", "Other"});

    QLineEdit *descInput = new QLineEdit(&dlg);

    QDoubleSpinBox *amountSpin = new QDoubleSpinBox(&dlg);
    amountSpin->setRange(1.0, 1000000.0);
    amountSpin->setValue(5000.0);
    amountSpin->setPrefix("Rs. ");

    QDateEdit *dateEdit = new QDateEdit(&dlg);
    dateEdit->setDisplayFormat("yyyy-MM-dd");
    dateEdit->setDate(QDate::currentDate());
    dateEdit->setCalendarPopup(true);

    QComboBox *methodCombo = new QComboBox(&dlg);
    methodCombo->addItems({"Cash", "Bank Transfer", "Cheque", "Card"});

    form->addRow("Expense Category *:", catCombo);
    form->addRow("Description *:", descInput);
    form->addRow("Amount (PKR) *:", amountSpin);
    form->addRow("Expense Date *:", dateEdit);
    form->addRow("Payment Method:", methodCombo);

    layout->addLayout(form);

    QHBoxLayout *btnBox = new QHBoxLayout();
    QPushButton *cancelBtn = new QPushButton("Cancel", &dlg);
    cancelBtn->setObjectName("secondaryBtn");
    QPushButton *saveBtn = new QPushButton("Save Expense", &dlg);
    saveBtn->setObjectName("successBtn");

    btnBox->addWidget(cancelBtn);
    btnBox->addWidget(saveBtn);
    layout->addLayout(btnBox);

    connect(cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, [&]() {
        if (descInput->text().trimmed().isEmpty()) return;

        Expense e;
        e.setCategory(catCombo->currentText());
        e.setDescription(descInput->text().trimmed());
        e.setAmount(amountSpin->value());
        e.setExpenseDate(dateEdit->date().toString("yyyy-MM-dd"));
        e.setPaymentMethod(methodCombo->currentText());
        e.setAddedBy(1);

        if (m_expenseRepo.create(e)) {
            dlg.accept();
            refreshData();
        }
    });

    dlg.exec();
}

} // namespace FitCore
