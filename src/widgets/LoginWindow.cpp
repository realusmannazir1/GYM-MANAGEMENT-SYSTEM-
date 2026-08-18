#include "widgets/LoginWindow.h"
#include "services/AuthenticationService.h"
#include "utils/ThemeManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QApplication>
#include <QMessageBox>

namespace FitCore {

LoginWindow::LoginWindow(QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("FitCore Enterprise — Login Account Portal");
    resize(860, 520);
    setMinimumSize(720, 460);
    setStyleSheet(ThemeManager::getLightThemeQss());
    setWindowFlags(Qt::Window | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);

    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(20, 20, 20, 20);

    // Studio Dual-Panel Card Frame
    QFrame *cardFrame = new QFrame(this);
    cardFrame->setObjectName("loginCardFrame");
    cardFrame->setStyleSheet(R"(
        QFrame#loginCardFrame {
            background-color: #FFFFFF;
            border: 1px solid #CBD5E1;
            border-radius: 16px;
        }
    )");

    QHBoxLayout *splitLayout = new QHBoxLayout(cardFrame);
    splitLayout->setContentsMargins(0, 0, 0, 0);
    splitLayout->setSpacing(0);

    // ==========================================
    // LEFT PANEL: Vibrant Blue Gradient Art & Welcome Hero
    // ==========================================
    QFrame *leftHeroPanel = new QFrame(cardFrame);
    leftHeroPanel->setMinimumWidth(380);
    leftHeroPanel->setStyleSheet(R"(
        QFrame {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #1D4ED8, stop:0.5 #2563EB, stop:1 #3B82F6);
            border-top-left-radius: 15px;
            border-bottom-left-radius: 15px;
            border-top-right-radius: 0px;
            border-bottom-right-radius: 0px;
        }
    )");

    QVBoxLayout *heroLayout = new QVBoxLayout(leftHeroPanel);
    heroLayout->setContentsMargins(36, 40, 36, 40);
    heroLayout->setSpacing(16);

    QLabel *topTag = new QLabel("🛡️ FITCORE ENTERPRISE", leftHeroPanel);
    topTag->setStyleSheet("color: #93C5FD; font-weight: 800; font-size: 12px; letter-spacing: 2px;");

    heroLayout->addWidget(topTag);
    heroLayout->addStretch(1);

    QLabel *niceSeeLbl = new QLabel("Nice to see you again", leftHeroPanel);
    niceSeeLbl->setStyleSheet("color: #E0F2FE; font-size: 16px; font-weight: 500;");

    QLabel *welcomeLbl = new QLabel("WELCOME BACK", leftHeroPanel);
    welcomeLbl->setStyleSheet("color: #FFFFFF; font-size: 34px; font-weight: 900; letter-spacing: 2px;");

    QFrame *dividerLine = new QFrame(leftHeroPanel);
    dividerLine->setFixedHeight(4);
    dividerLine->setFixedWidth(60);
    dividerLine->setStyleSheet("background-color: #60A5FA; border-radius: 2px;");

    QLabel *descLbl = new QLabel("Streamline your gym operations, member subscriptions, trainer schedules, and real-time analytical reporting with FitCore Studio.", leftHeroPanel);
    descLbl->setWordWrap(true);
    descLbl->setStyleSheet("color: #DBEAFE; font-size: 13px; line-height: 1.5; padding-top: 8px;");

    heroLayout->addWidget(niceSeeLbl);
    heroLayout->addWidget(welcomeLbl);
    heroLayout->addWidget(dividerLine);
    heroLayout->addWidget(descLbl);

    heroLayout->addStretch(2);

    QLabel *footerTag = new QLabel("⚡ FitCore v1.0.0 Enterprise • High Performance Edition", leftHeroPanel);
    footerTag->setStyleSheet("color: #BFDBFE; font-size: 11px; font-weight: 500;");
    heroLayout->addWidget(footerTag);

    splitLayout->addWidget(leftHeroPanel, 1);

    // ==========================================
    // RIGHT PANEL: Clean Login Form Controls
    // ==========================================
    QWidget *rightFormWidget = new QWidget(cardFrame);
    QVBoxLayout *formLayout = new QVBoxLayout(rightFormWidget);
    formLayout->setContentsMargins(40, 40, 40, 40);
    formLayout->setSpacing(16);

    QLabel *formTitle = new QLabel("Login Account", rightFormWidget);
    formTitle->setStyleSheet("font-size: 28px; font-weight: 800; color: #1E3A8A;");

    QLabel *formSubtitle = new QLabel("Select your role preset or enter credentials below", rightFormWidget);
    formSubtitle->setStyleSheet("font-size: 13px; color: #64748B; padding-bottom: 4px;");

    formLayout->addWidget(formTitle);
    formLayout->addWidget(formSubtitle);

    // Role Preset Selector Dropdown
    QHBoxLayout *presetLayout = new QHBoxLayout();
    QLabel *presetLbl = new QLabel("Role Preset:", rightFormWidget);
    presetLbl->setStyleSheet("font-weight: bold; color: #334155; font-size: 12px;");

    m_rolePresetCombo = new QComboBox(rightFormWidget);
    m_rolePresetCombo->setStyleSheet("padding: 8px 10px; font-weight: 600; border: 1px solid #CBD5E1; border-radius: 8px; background: #F8FAFC;");
    m_rolePresetCombo->addItem("👑 Administrator (admin)");
    m_rolePresetCombo->addItem("👔 Branch Manager (manager)");
    m_rolePresetCombo->addItem("📋 Reception Desk (receptionist)");
    m_rolePresetCombo->addItem("💪 Personal Trainer (trainer1)");

    presetLayout->addWidget(presetLbl);
    presetLayout->addWidget(m_rolePresetCombo, 1);
    formLayout->addLayout(presetLayout);

    // Username Field
    QLabel *userLbl = new QLabel("Username", rightFormWidget);
    userLbl->setStyleSheet("font-weight: bold; color: #1E293B; font-size: 12px;");
    m_usernameInput = new QLineEdit(rightFormWidget);
    m_usernameInput->setPlaceholderText("Enter your username");
    m_usernameInput->setText("admin");
    m_usernameInput->setStyleSheet("border: 1px solid #CBD5E1; border-left: 4px solid #2563EB; border-radius: 8px; padding: 10px 14px; font-size: 13px; background-color: #F8FAFC;");

    // Password Field
    QLabel *passLbl = new QLabel("Password", rightFormWidget);
    passLbl->setStyleSheet("font-weight: bold; color: #1E293B; font-size: 12px;");
    m_passwordInput = new QLineEdit(rightFormWidget);
    m_passwordInput->setPlaceholderText("Enter your password");
    m_passwordInput->setEchoMode(QLineEdit::Password);
    m_passwordInput->setText("admin123");
    m_passwordInput->setStyleSheet("border: 1px solid #CBD5E1; border-left: 4px solid #2563EB; border-radius: 8px; padding: 10px 14px; font-size: 13px; background-color: #F8FAFC;");

    formLayout->addWidget(userLbl);
    formLayout->addWidget(m_usernameInput);
    formLayout->addWidget(passLbl);
    formLayout->addWidget(m_passwordInput);

    // Error Alert Banner
    m_errorLabel = new QLabel("", rightFormWidget);
    m_errorLabel->setAlignment(Qt::AlignCenter);
    m_errorLabel->setStyleSheet("color: #DC2626; font-size: 12px; font-weight: bold; background: #FEE2E2; border-radius: 6px; padding: 6px;");
    m_errorLabel->hide();
    formLayout->addWidget(m_errorLabel);

    formLayout->addStretch();

    // Primary Action Button (Sleek Rounded Pill Button)
    m_loginBtn = new QPushButton("SIGN IN TO DASHBOARD", rightFormWidget);
    m_loginBtn->setCursor(Qt::PointingHandCursor);
    m_loginBtn->setStyleSheet("padding: 12px; font-size: 14px; font-weight: 800; background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 #2563EB, stop:1 #1D4ED8); color: #FFFFFF; border-radius: 22px; letter-spacing: 1px;");

    m_exitBtn = new QPushButton("Exit Application", rightFormWidget);
    m_exitBtn->setObjectName("secondaryBtn");
    m_exitBtn->setStyleSheet("padding: 8px; font-size: 12px; color: #64748B;");

    formLayout->addWidget(m_loginBtn);
    formLayout->addWidget(m_exitBtn);

    splitLayout->addWidget(rightFormWidget, 1);

    outerLayout->addWidget(cardFrame);

    connect(m_loginBtn, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(m_exitBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_passwordInput, &QLineEdit::returnPressed, this, &LoginWindow::onLoginClicked);
    connect(m_usernameInput, &QLineEdit::returnPressed, this, &LoginWindow::onLoginClicked);
    connect(m_rolePresetCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LoginWindow::onRolePresetChanged);
}

std::optional<User> LoginWindow::getAuthenticatedUser() const {
    return m_authenticatedUser;
}

void LoginWindow::onRolePresetChanged(int index) {
    if (index == 0) {
        m_usernameInput->setText("admin");
        m_passwordInput->setText("admin123");
    } else if (index == 1) {
        m_usernameInput->setText("manager");
        m_passwordInput->setText("manager123");
    } else if (index == 2) {
        m_usernameInput->setText("receptionist");
        m_passwordInput->setText("receptionist123");
    } else if (index == 3) {
        m_usernameInput->setText("trainer1");
        m_passwordInput->setText("trainer123");
    }
}

void LoginWindow::onLoginClicked() {
    m_errorLabel->setText("");
    m_errorLabel->hide();

    QString username = m_usernameInput->text().trimmed();
    QString password = m_passwordInput->text().trimmed();

    if (username.isEmpty() || password.isEmpty()) {
        m_errorLabel->setText("⚠️ Please enter both username and password.");
        m_errorLabel->show();
        return;
    }

    AuthResult result = AuthenticationService::instance().login(username, password);

    if (result.success) {
        m_authenticatedUser = result.user;
        accept();
    } else {
        m_errorLabel->setText("⚠️ " + result.message);
        m_errorLabel->show();
    }
}

} // namespace FitCore
