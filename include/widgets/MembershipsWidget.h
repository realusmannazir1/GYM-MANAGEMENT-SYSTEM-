#ifndef MEMBERSHIPSWIDGET_H
#define MEMBERSHIPSWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include "services/MembershipService.h"
#include "repositories/MembershipRepository.h"
#include "repositories/MembershipPlanRepository.h"
#include "repositories/MemberRepository.h"

namespace FitCore {

class MembershipsWidget : public QWidget {
    Q_OBJECT
public:
    explicit MembershipsWidget(QWidget *parent = nullptr);

    void refreshData();

private slots:
    void onRenewClicked();
    void onAddPlanClicked();
    void onCancelMembershipClicked(int membershipId);

private:
    void setupUi();
    void loadActiveTable();
    void loadExpiringTable();
    void loadExpiredTable();
    void loadPlansTable();

    QTableWidget *m_activeTable;
    QTableWidget *m_expiringTable;
    QTableWidget *m_expiredTable;
    QTableWidget *m_plansTable;

    MembershipService m_membershipService;
    MembershipRepository m_membershipRepo;
    MembershipPlanRepository m_planRepo;
    MemberRepository m_memberRepo;
};

} // namespace FitCore

#endif // MEMBERSHIPSWIDGET_H
