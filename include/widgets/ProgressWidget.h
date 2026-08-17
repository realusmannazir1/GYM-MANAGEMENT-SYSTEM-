#ifndef PROGRESSWIDGET_H
#define PROGRESSWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include "repositories/ProgressRepository.h"
#include "repositories/MemberRepository.h"

namespace FitCore {

class ProgressWidget : public QWidget {
    Q_OBJECT
public:
    explicit ProgressWidget(QWidget *parent = nullptr);

    void refreshData();

private slots:
    void onAddRecordClicked();

private:
    void setupUi();
    void loadProgressTable();

    QComboBox *m_memberFilter;
    QTableWidget *m_table;

    ProgressRepository m_progressRepo;
    MemberRepository m_memberRepo;
};

} // namespace FitCore

#endif // PROGRESSWIDGET_H
