#ifndef REPORTSWIDGET_H
#define REPORTSWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include "services/ReportService.h"

namespace FitCore {

class ReportsWidget : public QWidget {
    Q_OBJECT
public:
    explicit ReportsWidget(QWidget *parent = nullptr);

    void refreshData();

private slots:
    void onGenerateReport();
    void onExportCSV();

private:
    void setupUi();

    QComboBox *m_reportTypeCombo;
    QTextEdit *m_previewText;

    ReportService m_reportService;
};

} // namespace FitCore

#endif // REPORTSWIDGET_H
