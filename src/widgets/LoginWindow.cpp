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
    setWindowTitle("FitCore — Gym Management System Login");
    setFixedSize(450, 420);
    setStyleSheet(ThemeManager::getLightThemeQss());
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    // Title & Logo Label
    QLabel *titleLabel = new QLabel("FitCore", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 32px; font-weight: bold; color: #1E40AF; letter-spacing: 1px;");

    QLabel *subtitleLabel = new QLabel("Gym Management System", this);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("font-size: 14px; color: #475569; font-weight: 600;");

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(subtitleLabel);

    // Form
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(12);

    m_usernameInput = new QLineEdit(this);
    m_usernameInput->setPlaceholderText("Enter username");
    m_usernameInput->setText("admin"); // Default hint

    m_passwordInput = new QLineEdit(this);
    m_passwordInput->setPlaceholderText("Enter password");
    m_passwordInput->setEchoMode(QLineEdit::Password);
    m_passwordInput->setText("admin123"); // Default hint

    formLayout->addRow("Username:", m_usernameInput);
    formLayout->addRow("Password:", m_passwordInput);
    mainLayout->addLayout(formLayout);

    // Error Label
    m_errorLabel = new QLabel("", this);
    m_errorLabel->setAlignment(Qt::AlignCenter);
    m_errorLabel->setStyleSheet("color: #EF4444; font-size: 12px; font-weight: bold;");
    mainLayout->addWidget(m_errorLabel);

    // Demo Credentials Box
    QLabel *demoLabel = new QLabel("Demo Logins:\nadmin / admin123  |  manager / manager123\nreceptionist / receptionist123  |  trainer1 / trainer123", this);
    demoLabel->setAlignment(Qt::AlignCenter);
    demoLabel->setStyleSheet("color: #1E40AF; font-size: 11px; background-color: #EFF6FF; border: 1px solid #BFDBFE; padding: 8px; border-radius: 6px;");
    mainLayout->addWidget(demoLabel);

    // Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    m_exitBtn = new QPushButton("Exit", this);
    m_exitBtn->setObjectName("secondaryBtn");
    m_loginBtn = new QPushButton("Login", this);

    btnLayout->addWidget(m_exitBtn);
    btnLayout->addWidget(m_loginBtn);
    mainLayout->addLayout(btnLayout);

    connect(m_loginBtn, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(m_exitBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_passwordInput, &QLineEdit::returnPressed, this, &LoginWindow::onLoginClicked);
    connect(m_usernameInput, &QLineEdit::returnPressed, this, &LoginWindow::onLoginClicked);
}

std::optional<User> LoginWindow::getAuthenticatedUser() const {
    return m_authenticatedUser;
}

void LoginWindow::onLoginClicked() {
    m_errorLabel->setText("");
    QString username = m_usernameInput->text();
    QString password = m_passwordInput->text();

    AuthResult result = AuthenticationService::instance().login(username, password);

    if (result.success) {
        m_authenticatedUser = result.user;
        accept();
    } else {
        m_errorLabel->setText(result.message);
    }
}

} // namespace FitCore
