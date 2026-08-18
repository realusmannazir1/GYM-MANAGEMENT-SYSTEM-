#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include "models/User.h"

namespace FitCore {

class LoginWindow : public QDialog {
    Q_OBJECT
public:
    explicit LoginWindow(QWidget *parent = nullptr);

    std::optional<User> getAuthenticatedUser() const;

private slots:
    void onLoginClicked();
    void onRolePresetChanged(int index);

private:
    QLineEdit *m_usernameInput;
    QLineEdit *m_passwordInput;
    QPushButton *m_loginBtn;
    QPushButton *m_exitBtn;
    QLabel *m_errorLabel;
    QComboBox *m_rolePresetCombo;

    std::optional<User> m_authenticatedUser;
};

} // namespace FitCore

#endif // LOGINWINDOW_H
