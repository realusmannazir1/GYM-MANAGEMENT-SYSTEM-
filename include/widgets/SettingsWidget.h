#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "services/BackupService.h"
#include "repositories/SettingsRepository.h"

namespace FitCore {

class SettingsWidget : public QWidget {
    Q_OBJECT
public:
    explicit SettingsWidget(QWidget *parent = nullptr);

    void refreshData();

private slots:
    void onSaveSettingsClicked();
    void onCreateBackupClicked();
    void onRestoreBackupClicked();

private:
    void setupUi();

    QLineEdit *m_gymNameInput;
    QLineEdit *m_phoneInput;
    QLineEdit *m_emailInput;
    QLineEdit *m_addressInput;
    QLineEdit *m_currencyInput;
    QLineEdit *m_taxRateInput;

    QLabel *m_backupStatusLbl;

    BackupService m_backupService;
    SettingsRepository m_settingsRepo;
};

} // namespace FitCore

#endif // SETTINGSWIDGET_H
