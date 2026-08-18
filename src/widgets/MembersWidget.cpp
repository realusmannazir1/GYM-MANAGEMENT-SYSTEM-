#include "widgets/MembersWidget.h"
#include "utils/ThemeManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QDialog>
#include <QMessageBox>
#include <QTabWidget>
#include <QDateEdit>

namespace FitCore {

MembersWidget::MembersWidget(QWidget *parent)
    : QWidget(parent) {
    setupUi();
    refreshData();
}

void MembersWidget::setupUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    // Section 1: Header Bar & Actions
    QHBoxLayout *topLayout = new QHBoxLayout();
    QLabel *title = new QLabel("Member Directory", this);
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #0F172A;");

    QPushButton *addMemberBtn = new QPushButton("+ Add New Member", this);
    addMemberBtn->setObjectName("successBtn");

    topLayout->addWidget(title);
    topLayout->addStretch();
    topLayout->addWidget(addMemberBtn);
    mainLayout->addLayout(topLayout);

    // Section 2: Search & Filter Controls
    QHBoxLayout *filterLayout = new QHBoxLayout();
    m_searchInput = new QLineEdit(this);
    m_searchInput->setPlaceholderText("Search by Name, Phone, Email, or Membership #...");
    m_searchInput->setClearButtonEnabled(true);

    m_statusFilter = new QComboBox(this);
    m_statusFilter->addItems({"All Statuses", "Active", "Inactive", "Archived"});

    m_rowsPerPageCombo = new QComboBox(this);
    m_rowsPerPageCombo->addItems({"10 rows/page", "20 rows/page", "50 rows/page"});
    m_rowsPerPageCombo->setCurrentIndex(1); // Default 20

    filterLayout->addWidget(m_searchInput, 3);
    filterLayout->addWidget(m_statusFilter, 1);
    filterLayout->addWidget(m_rowsPerPageCombo, 1);
    mainLayout->addLayout(filterLayout);

    // Section 3: Table
    m_table = new QTableWidget(this);
    m_table->setColumnCount(7);
    m_table->setHorizontalHeaderLabels({"Membership #", "Full Name", "Gender", "Phone", "Registration Date", "Status", "Actions"});
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    mainLayout->addWidget(m_table);

    // Section 4: Pagination Bar
    QHBoxLayout *pageLayout = new QHBoxLayout();
    m_prevBtn = new QPushButton("Previous", this);
    m_prevBtn->setObjectName("secondaryBtn");
    m_nextBtn = new QPushButton("Next", this);
    m_nextBtn->setObjectName("secondaryBtn");

    m_pageLabel = new QLabel("Page 1 of 1", this);
    m_pageLabel->setStyleSheet("color: #64748B; font-weight: 600;");

    pageLayout->addWidget(m_pageLabel);
    pageLayout->addStretch();
    pageLayout->addWidget(m_prevBtn);
    pageLayout->addWidget(m_nextBtn);
    mainLayout->addLayout(pageLayout);

    // Signal/Slot Connects
    connect(addMemberBtn, &QPushButton::clicked, this, &MembersWidget::onAddMemberClicked);
    connect(m_searchInput, &QLineEdit::textChanged, this, &MembersWidget::onSearchTextChanged);
    connect(m_statusFilter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MembersWidget::onFilterChanged);
    connect(m_rowsPerPageCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int idx) {
        if (idx == 0) m_rowsPerPage = 10;
        else if (idx == 1) m_rowsPerPage = 20;
        else if (idx == 2) m_rowsPerPage = 50;
        m_currentPage = 1;
        loadMembersTable();
    });
    connect(m_prevBtn, &QPushButton::clicked, this, &MembersWidget::onPrevPage);
    connect(m_nextBtn, &QPushButton::clicked, this, &MembersWidget::onNextPage);
}

void MembersWidget::refreshData() {
    loadMembersTable();
}

void MembersWidget::loadMembersTable() {
    QString keyword = m_searchInput->text().trimmed();
    QString filter = m_statusFilter->currentText();
    if (filter == "All Statuses") filter = "";

    std::vector<Member> members;
    if (!keyword.isEmpty()) {
        members = m_memberRepo.search(keyword);
        m_totalMembersCount = static_cast<int>(members.size());
    } else {
        int offset = (m_currentPage - 1) * m_rowsPerPage;
        members = m_memberRepo.findAllPaged(m_rowsPerPage, offset, filter);
        m_totalMembersCount = m_memberRepo.count(filter);
    }

    int totalPages = (m_totalMembersCount + m_rowsPerPage - 1) / m_rowsPerPage;
    if (totalPages < 1) totalPages = 1;
    m_pageLabel->setText(QString("Page %1 of %2 (Total %3 members)").arg(m_currentPage).arg(totalPages).arg(m_totalMembersCount));

    m_prevBtn->setEnabled(m_currentPage > 1);
    m_nextBtn->setEnabled(m_currentPage < totalPages);

    m_table->setRowCount(0);
    int r = 0;
    for (const auto& m : members) {
        m_table->insertRow(r);
        m_table->setItem(r, 0, new QTableWidgetItem(m.getMembershipNumber()));
        m_table->setItem(r, 1, new QTableWidgetItem(m.getFullName()));
        m_table->setItem(r, 2, new QTableWidgetItem(m.getGender()));
        m_table->setItem(r, 3, new QTableWidgetItem(m.getPhone()));
        m_table->setItem(r, 4, new QTableWidgetItem(m.getRegistrationDate()));

        QLabel *statusBadge = new QLabel(m.getProfileStatus(), this);
        statusBadge->setStyleSheet(ThemeManager::getBadgeStyle(m.getProfileStatus()));
        statusBadge->setAlignment(Qt::AlignCenter);
        m_table->setCellWidget(r, 5, statusBadge);

        // Action Buttons Widget
        QWidget *actionWidget = new QWidget(this);
        QHBoxLayout *actLayout = new QHBoxLayout(actionWidget);
        actLayout->setContentsMargins(4, 2, 4, 2);
        actLayout->setSpacing(4);

        QPushButton *viewBtn = new QPushButton("View", actionWidget);
        viewBtn->setObjectName("secondaryBtn");
        viewBtn->setStyleSheet("padding: 4px 8px; font-size: 11px;");

        QPushButton *editBtn = new QPushButton("Edit", actionWidget);
        editBtn->setStyleSheet("padding: 4px 8px; font-size: 11px;");

        QPushButton *delBtn = new QPushButton("Archive", actionWidget);
        delBtn->setObjectName("dangerBtn");
        delBtn->setStyleSheet("padding: 4px 8px; font-size: 11px;");

        int id = m.getId();
        connect(viewBtn, &QPushButton::clicked, this, [this, id]() { onViewProfileClicked(id); });
        connect(editBtn, &QPushButton::clicked, this, [this, id]() { onEditMemberClicked(id); });
        connect(delBtn, &QPushButton::clicked, this, [this, id]() { onDeleteMemberClicked(id); });

        actLayout->addWidget(viewBtn);
        actLayout->addWidget(editBtn);
        actLayout->addWidget(delBtn);
        m_table->setCellWidget(r, 6, actionWidget);
        r++;
    }
}

void MembersWidget::onSearchTextChanged(const QString&) {
    m_currentPage = 1;
    loadMembersTable();
}

void MembersWidget::onFilterChanged() {
    m_currentPage = 1;
    loadMembersTable();
}

void MembersWidget::onPrevPage() {
    if (m_currentPage > 1) {
        m_currentPage--;
        loadMembersTable();
    }
}

void MembersWidget::onNextPage() {
    m_currentPage++;
    loadMembersTable();
}

void MembersWidget::onAddMemberClicked() {
    openMemberDialog();
}

void MembersWidget::onEditMemberClicked(int memberId) {
    auto opt = m_memberRepo.findById(memberId);
    if (opt.has_value()) {
        openMemberDialog(opt);
    }
}

void MembersWidget::onDeleteMemberClicked(int memberId) {
    auto opt = m_memberRepo.findById(memberId);
    if (!opt.has_value()) return;

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Archive Member",
        QString("Are you sure you want to archive member '%1' (%2)?\nHistorical records will be preserved.")
            .arg(opt->getFullName(), opt->getMembershipNumber()),
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        m_memberRepo.setStatus(memberId, "Archived");
        loadMembersTable();
    }
}

void MembersWidget::openMemberDialog(const std::optional<Member>& memberOpt) {
    QDialog dlg(this);
    bool isEdit = memberOpt.has_value();
    dlg.setWindowTitle(isEdit ? "Edit Member Details" : "Register New Member");
    dlg.resize(520, 600);
    dlg.setMinimumSize(440, 420);
    dlg.setStyleSheet(ThemeManager::getLightThemeQss());
    dlg.setWindowFlags(Qt::Window | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);

    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    QFormLayout *form = new QFormLayout();

    QLineEdit *memNumInput = new QLineEdit(&dlg);
    memNumInput->setText(isEdit ? memberOpt->getMembershipNumber() : m_memberRepo.generateNextMembershipNumber());
    memNumInput->setReadOnly(true);

    QLineEdit *fnInput = new QLineEdit(&dlg);
    if (isEdit) fnInput->setText(memberOpt->getFirstName());

    QLineEdit *lnInput = new QLineEdit(&dlg);
    if (isEdit) lnInput->setText(memberOpt->getLastName());

    QComboBox *genderCombo = new QComboBox(&dlg);
    genderCombo->addItems({"Male", "Female", "Other"});
    if (isEdit) genderCombo->setCurrentText(memberOpt->getGender());

    QDateEdit *dobEdit = new QDateEdit(&dlg);
    dobEdit->setDisplayFormat("yyyy-MM-dd");
    dobEdit->setCalendarPopup(true);
    dobEdit->setDate(isEdit && !memberOpt->getDateOfBirth().isEmpty() ? QDate::fromString(memberOpt->getDateOfBirth(), "yyyy-MM-dd") : QDate(1998, 1, 1));

    QLineEdit *phoneInput = new QLineEdit(&dlg);
    if (isEdit) phoneInput->setText(memberOpt->getPhone());

    QLineEdit *emailInput = new QLineEdit(&dlg);
    if (isEdit) emailInput->setText(memberOpt->getEmail());

    QLineEdit *addrInput = new QLineEdit(&dlg);
    if (isEdit) addrInput->setText(memberOpt->getAddress());

    QLineEdit *emContactInput = new QLineEdit(&dlg);
    if (isEdit) emContactInput->setText(memberOpt->getEmergencyContact());

    QLineEdit *emPhoneInput = new QLineEdit(&dlg);
    if (isEdit) emPhoneInput->setText(memberOpt->getEmergencyPhone());

    QComboBox *statusCombo = new QComboBox(&dlg);
    statusCombo->addItems({"Active", "Inactive", "Archived"});
    if (isEdit) statusCombo->setCurrentText(memberOpt->getProfileStatus());

    form->addRow("Membership #:", memNumInput);
    form->addRow("First Name *:", fnInput);
    form->addRow("Last Name *:", lnInput);
    form->addRow("Gender:", genderCombo);
    form->addRow("Date of Birth:", dobEdit);
    form->addRow("Phone Number *:", phoneInput);
    form->addRow("Email Address:", emailInput);
    form->addRow("Address:", addrInput);
    form->addRow("Emergency Contact:", emContactInput);
    form->addRow("Emergency Phone:", emPhoneInput);
    form->addRow("Profile Status:", statusCombo);

    layout->addLayout(form);

    QHBoxLayout *btnBox = new QHBoxLayout();
    QPushButton *cancelBtn = new QPushButton("Cancel", &dlg);
    cancelBtn->setObjectName("secondaryBtn");
    QPushButton *saveBtn = new QPushButton(isEdit ? "Save Changes" : "Register Member", &dlg);
    saveBtn->setObjectName("successBtn");

    btnBox->addWidget(cancelBtn);
    btnBox->addWidget(saveBtn);
    layout->addLayout(btnBox);

    connect(cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, [&]() {
        if (fnInput->text().trimmed().isEmpty() || phoneInput->text().trimmed().isEmpty()) {
            QMessageBox::warning(&dlg, "Validation Error", "First Name and Phone Number are required fields.");
            return;
        }

        Member m = isEdit ? memberOpt.value() : Member();
        m.setMembershipNumber(memNumInput->text());
        m.setFirstName(fnInput->text().trimmed());
        m.setLastName(lnInput->text().trimmed());
        m.setGender(genderCombo->currentText());
        m.setDateOfBirth(dobEdit->date().toString("yyyy-MM-dd"));
        m.setPhone(phoneInput->text().trimmed());
        m.setEmail(emailInput->text().trimmed());
        m.setAddress(addrInput->text().trimmed());
        m.setEmergencyContact(emContactInput->text().trimmed());
        m.setEmergencyPhone(emPhoneInput->text().trimmed());
        m.setProfileStatus(statusCombo->currentText());

        bool ok = isEdit ? m_memberRepo.update(m) : m_memberRepo.create(m);
        if (ok) {
            dlg.accept();
            loadMembersTable();
        } else {
            QMessageBox::critical(&dlg, "Database Error", "Failed to save member details into database.");
        }
    });

    dlg.exec();
}

void MembersWidget::onViewProfileClicked(int memberId) {
    openMemberProfileDialog(memberId);
}

void MembersWidget::openMemberProfileDialog(int memberId) {
    auto memberOpt = m_memberRepo.findById(memberId);
    if (!memberOpt.has_value()) return;
    Member member = memberOpt.value();

    QDialog dlg(this);
    dlg.setWindowTitle("Member Profile Dashboard — " + member.getFullName() + " (" + member.getMembershipNumber() + ")");
    dlg.resize(750, 550);
    dlg.setMinimumSize(550, 420);
    dlg.setStyleSheet(ThemeManager::getLightThemeQss());
    dlg.setWindowFlags(Qt::Window | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);

    QVBoxLayout *layout = new QVBoxLayout(&dlg);

    // Profile Top Header Banner
    QFrame *topFrame = new QFrame(&dlg);
    topFrame->setObjectName("statCard");
    QHBoxLayout *topLayout = new QHBoxLayout(topFrame);

    QVBoxLayout *infoCol = new QVBoxLayout();
    QLabel *nameLbl = new QLabel(member.getFullName(), topFrame);
    nameLbl->setStyleSheet("font-size: 20px; font-weight: bold; color: #0F172A;");

    QLabel *subLbl = new QLabel(QString("Membership: %1 | Registered: %2 | Phone: %3")
                                   .arg(member.getMembershipNumber(), member.getRegistrationDate(), member.getPhone()), topFrame);
    subLbl->setStyleSheet("color: #64748B; font-size: 12px;");

    infoCol->addWidget(nameLbl);
    infoCol->addWidget(subLbl);

    QLabel *badge = new QLabel(member.getProfileStatus(), topFrame);
    badge->setStyleSheet(ThemeManager::getBadgeStyle(member.getProfileStatus()));
    badge->setAlignment(Qt::AlignCenter);

    topLayout->addLayout(infoCol);
    topLayout->addStretch();
    topLayout->addWidget(badge);

    layout->addWidget(topFrame);

    // Profile Details Tabs
    QTabWidget *tabs = new QTabWidget(&dlg);

    // Tab 1: Active Membership & Plan History
    QWidget *tabMs = new QWidget();
    QVBoxLayout *msLayout = new QVBoxLayout(tabMs);
    QTableWidget *msTable = new QTableWidget(tabMs);
    msTable->setColumnCount(5);
    msTable->setHorizontalHeaderLabels({"Plan", "Start Date", "End Date", "Amount", "Status"});
    msTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    MembershipRepository msRepo;
    auto msList = msRepo.findHistoryByMemberId(memberId);
    msTable->setRowCount(0);
    int r = 0;
    for (const auto& ms : msList) {
        msTable->insertRow(r);
        msTable->setItem(r, 0, new QTableWidgetItem(ms.getPlanName()));
        msTable->setItem(r, 1, new QTableWidgetItem(ms.getStartDate()));
        msTable->setItem(r, 2, new QTableWidgetItem(ms.getEndDate()));
        msTable->setItem(r, 3, new QTableWidgetItem(QString("Rs. %1").arg(ms.getFinalAmount(), 0, 'f', 0)));
        msTable->setItem(r, 4, new QTableWidgetItem(ms.getStatus()));
        r++;
    }
    msLayout->addWidget(msTable);
    tabs->addTab(tabMs, "Membership History");

    // Tab 2: Attendance History
    QWidget *tabAtt = new QWidget();
    QVBoxLayout *attLayout = new QVBoxLayout(tabAtt);
    QTableWidget *attTable = new QTableWidget(tabAtt);
    attTable->setColumnCount(4);
    attTable->setHorizontalHeaderLabels({"Date", "Check-In", "Check-Out", "Duration"});
    attTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    AttendanceRepository attRepo;
    auto attList = attRepo.findByMemberId(memberId);
    attTable->setRowCount(0);
    r = 0;
    for (const auto& a : attList) {
        attTable->insertRow(r);
        attTable->setItem(r, 0, new QTableWidgetItem(a.getAttendanceDate()));
        attTable->setItem(r, 1, new QTableWidgetItem(a.getCheckInTime()));
        attTable->setItem(r, 2, new QTableWidgetItem(a.getCheckOutTime()));
        attTable->setItem(r, 3, new QTableWidgetItem(a.getDurationFormatted()));
        r++;
    }
    attLayout->addWidget(attTable);
    tabs->addTab(tabAtt, QString("Attendance (%1 Visits)").arg(attList.size()));

    // Tab 3: Payment History
    QWidget *tabPay = new QWidget();
    QVBoxLayout *payLayout = new QVBoxLayout(tabPay);
    QTableWidget *payTable = new QTableWidget(tabPay);
    payTable->setColumnCount(5);
    payTable->setHorizontalHeaderLabels({"Receipt", "Date", "Amount", "Method", "Notes"});
    payTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    PaymentRepository payRepo;
    auto payList = payRepo.findByMemberId(memberId);
    payTable->setRowCount(0);
    r = 0;
    for (const auto& p : payList) {
        payTable->insertRow(r);
        payTable->setItem(r, 0, new QTableWidgetItem(p.getReceiptNumber()));
        payTable->setItem(r, 1, new QTableWidgetItem(p.getPaymentDate()));
        payTable->setItem(r, 2, new QTableWidgetItem(QString("Rs. %1").arg(p.getFinalAmount(), 0, 'f', 0)));
        payTable->setItem(r, 3, new QTableWidgetItem(p.getPaymentMethod()));
        payTable->setItem(r, 4, new QTableWidgetItem(p.getNotes()));
        r++;
    }
    payLayout->addWidget(payTable);
    tabs->addTab(tabPay, "Payments & Receipts");

    // Tab 4: Body Progress
    QWidget *tabProg = new QWidget();
    QVBoxLayout *progLayout = new QVBoxLayout(tabProg);
    QTableWidget *progTable = new QTableWidget(tabProg);
    progTable->setColumnCount(6);
    progTable->setHorizontalHeaderLabels({"Date", "Weight (kg)", "Height (cm)", "BMI", "Body Fat %", "Waist (cm)"});
    progTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ProgressRepository progRepo;
    auto progList = progRepo.findByMemberId(memberId);
    progTable->setRowCount(0);
    r = 0;
    for (const auto& pr : progList) {
        progTable->insertRow(r);
        progTable->setItem(r, 0, new QTableWidgetItem(pr.getRecordDate()));
        progTable->setItem(r, 1, new QTableWidgetItem(QString::number(pr.getWeightKg(), 'f', 1)));
        progTable->setItem(r, 2, new QTableWidgetItem(QString::number(pr.getHeightCm(), 'f', 1)));
        progTable->setItem(r, 3, new QTableWidgetItem(QString("%1 (%2)").arg(pr.getBmi(), 0, 'f', 1).arg(pr.getBmiCategory())));
        progTable->setItem(r, 4, new QTableWidgetItem(QString::number(pr.getBodyFatPercentage(), 'f', 1)));
        progTable->setItem(r, 5, new QTableWidgetItem(QString::number(pr.getWaistCm(), 'f', 1)));
        r++;
    }
    progLayout->addWidget(progTable);
    tabs->addTab(tabProg, "Body Progress");

    layout->addWidget(tabs);

    QPushButton *closeBtn = new QPushButton("Close Profile", &dlg);
    closeBtn->setObjectName("secondaryBtn");
    connect(closeBtn, &QPushButton::clicked, &dlg, &QDialog::accept);
    layout->addWidget(closeBtn, 0, Qt::AlignRight);

    dlg.exec();
}

} // namespace FitCore
