#ifndef NOTIFICATIONSWIDGET_H
#define NOTIFICATIONSWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include "services/NotificationService.h"

namespace FitCore {

class NotificationsWidget : public QWidget {
    Q_OBJECT
public:
    explicit NotificationsWidget(QWidget *parent = nullptr);

    void refreshData();

private slots:
    void onMarkAllReadClicked();

private:
    void setupUi();
    void loadNotificationsTable();

    QTableWidget *m_table;

    NotificationService m_notificationService;
};

} // namespace FitCore

#endif // NOTIFICATIONSWIDGET_H
