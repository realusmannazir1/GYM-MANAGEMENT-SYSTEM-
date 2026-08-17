#ifndef ATTENDANCEWIDGET_H
#define ATTENDANCEWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include "services/AttendanceService.h"
#include "repositories/AttendanceRepository.h"

namespace FitCore {

class AttendanceWidget : public QWidget {
    Q_OBJECT
public:
    explicit AttendanceWidget(QWidget *parent = nullptr);

    void refreshData();

private slots:
    void onFastCheckIn();
    void onCheckOutClicked(int attendanceId);

private:
    void setupUi();
    void loadTodayAttendance();

    QLineEdit *m_checkInInput;
    QPushButton *m_checkInBtn;
    QLabel *m_statusFeedbackLbl;

    QLabel *m_todayCountLbl;
    QTableWidget *m_todayTable;

    AttendanceService m_attendanceService;
    AttendanceRepository m_attendanceRepo;
};

} // namespace FitCore

#endif // ATTENDANCEWIDGET_H
