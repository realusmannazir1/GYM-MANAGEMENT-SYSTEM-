#ifndef EQUIPMENTWIDGET_H
#define EQUIPMENTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include "repositories/EquipmentRepository.h"

namespace FitCore {

class EquipmentWidget : public QWidget {
    Q_OBJECT
public:
    explicit EquipmentWidget(QWidget *parent = nullptr);

    void refreshData();

private slots:
    void onAddEquipmentClicked();
    void onLogMaintenanceClicked(int equipmentId);

private:
    void setupUi();
    void loadEquipmentTable();
    void loadMaintenanceTable();

    QTableWidget *m_equipmentTable;
    QTableWidget *m_maintenanceTable;

    EquipmentRepository m_equipmentRepo;
};

} // namespace FitCore

#endif // EQUIPMENTWIDGET_H
