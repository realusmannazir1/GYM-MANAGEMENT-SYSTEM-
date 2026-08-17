#ifndef MEMBERSWIDGET_H
#define MEMBERSWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include "models/Member.h"
#include "repositories/MemberRepository.h"
#include "repositories/MembershipRepository.h"
#include "repositories/AttendanceRepository.h"
#include "repositories/PaymentRepository.h"
#include "repositories/WorkoutRepository.h"
#include "repositories/ProgressRepository.h"

namespace FitCore {

class MembersWidget : public QWidget {
    Q_OBJECT
public:
    explicit MembersWidget(QWidget *parent = nullptr);

    void refreshData();

private slots:
    void onSearchTextChanged(const QString& text);
    void onFilterChanged();
    void onAddMemberClicked();
    void onEditMemberClicked(int memberId);
    void onViewProfileClicked(int memberId);
    void onDeleteMemberClicked(int memberId);
    void onPrevPage();
    void onNextPage();

private:
    void setupUi();
    void loadMembersTable();
    void openMemberDialog(const std::optional<Member>& memberOpt = std::nullopt);
    void openMemberProfileDialog(int memberId);

    QLineEdit *m_searchInput;
    QComboBox *m_statusFilter;
    QComboBox *m_rowsPerPageCombo;
    QTableWidget *m_table;

    QLabel *m_pageLabel;
    QPushButton *m_prevBtn;
    QPushButton *m_nextBtn;

    int m_currentPage{1};
    int m_rowsPerPage{20};
    int m_totalMembersCount{0};

    MemberRepository m_memberRepo;
};

} // namespace FitCore

#endif // MEMBERSWIDGET_H
