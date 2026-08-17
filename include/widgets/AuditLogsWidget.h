#ifndef AUDITLOGSWIDGET_H
#define AUDITLOGSWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include "repositories/AuditRepository.h"

namespace FitCore {

class AuditLogsWidget : public QWidget {
    Q_OBJECT
public:
    explicit AuditLogsWidget(QWidget *parent = nullptr);

    void refreshData();

private:
    void setupUi();
    void loadAuditTable();

    QTableWidget *m_table;
    AuditRepository m_auditRepo;
};

} // namespace FitCore

#endif // AUDITLOGSWIDGET_H
