#include "utils/ThemeManager.h"

namespace FitCore {

QString ThemeManager::getDarkThemeQss() {
    return R"(
        /* Global Window & Typography */
        QWidget {
            background-color: #0F172A;
            color: #F8FAFC;
            font-family: 'Segoe UI', 'Roboto', sans-serif;
            font-size: 13px;
        }

        /* Sidebar Styling */
        QFrame#sidebarFrame {
            background-color: #1E293B;
            border-right: 1px solid #334155;
        }

        QListWidget#sidebarList {
            background-color: transparent;
            border: none;
            outline: none;
        }

        QListWidget#sidebarList::item {
            height: 42px;
            padding-left: 16px;
            color: #94A3B8;
            border-radius: 8px;
            margin: 3px 8px;
            font-weight: 600;
        }

        QListWidget#sidebarList::item:hover {
            background-color: #334155;
            color: #F8FAFC;
        }

        QListWidget#sidebarList::item:selected {
            background-color: #3B82F6;
            color: #FFFFFF;
        }

        /* Header Bar Styling */
        QFrame#headerFrame {
            background-color: #1E293B;
            border-bottom: 1px solid #334155;
        }

        QLabel#headerTitle {
            font-size: 18px;
            font-weight: bold;
            color: #F8FAFC;
        }

        QLabel#roleBadge {
            background-color: #3B82F6;
            color: #FFFFFF;
            padding: 4px 10px;
            border-radius: 12px;
            font-size: 11px;
            font-weight: bold;
        }

        /* Stat Cards */
        QFrame#statCard {
            background-color: #1E293B;
            border: 1px solid #334155;
            border-radius: 12px;
            padding: 16px;
        }

        QFrame#statCard:hover {
            border: 1px solid #3B82F6;
        }

        QLabel#statTitle {
            color: #94A3B8;
            font-size: 12px;
            font-weight: 600;
            text-transform: uppercase;
        }

        QLabel#statValue {
            color: #F8FAFC;
            font-size: 24px;
            font-weight: bold;
            margin-top: 4px;
        }

        /* Buttons */
        QPushButton {
            background-color: #3B82F6;
            color: #FFFFFF;
            border: none;
            border-radius: 6px;
            padding: 8px 16px;
            font-weight: 600;
            font-size: 13px;
        }

        QPushButton:hover {
            background-color: #2563EB;
        }

        QPushButton:pressed {
            background-color: #1D4ED8;
        }

        QPushButton#secondaryBtn {
            background-color: #334155;
            color: #F8FAFC;
        }

        QPushButton#secondaryBtn:hover {
            background-color: #475569;
        }

        QPushButton#dangerBtn {
            background-color: #EF4444;
            color: #FFFFFF;
        }

        QPushButton#dangerBtn:hover {
            background-color: #DC2626;
        }

        QPushButton#successBtn {
            background-color: #10B981;
            color: #FFFFFF;
        }

        QPushButton#successBtn:hover {
            background-color: #059669;
        }

        /* Input Controls */
        QLineEdit, QComboBox, QDateEdit, QSpinBox, QDoubleSpinBox, QTextEdit {
            background-color: #1E293B;
            border: 1px solid #334155;
            border-radius: 6px;
            padding: 8px 12px;
            color: #F8FAFC;
            selection-background-color: #3B82F6;
        }

        QLineEdit:focus, QComboBox:focus, QDateEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus, QTextEdit:focus {
            border: 1px solid #3B82F6;
        }

        /* Table Styling */
        QTableWidget {
            background-color: #1E293B;
            border: 1px solid #334155;
            gridline-color: #334155;
            border-radius: 8px;
        }

        QTableWidget::item {
            padding: 8px;
            color: #E2E8F0;
        }

        QTableWidget::item:selected {
            background-color: #334155;
            color: #3B82F6;
        }

        QHeaderView::section {
            background-color: #0F172A;
            color: #94A3B8;
            padding: 10px;
            font-weight: bold;
            border: none;
            border-bottom: 1px solid #334155;
        }

        /* Dialog Styling */
        QDialog {
            background-color: #0F172A;
        }

        /* Scrollbars */
        QScrollBar:vertical {
            background: #0F172A;
            width: 10px;
        }

        QScrollBar::handle:vertical {
            background: #334155;
            min-height: 20px;
            border-radius: 5px;
        }

        QScrollBar::handle:vertical:hover {
            background: #475569;
        }
    )";
}

QString ThemeManager::getLightThemeQss() {
    return getDarkThemeQss(); // Default dark theme for FitCore premium aesthetics
}

QString ThemeManager::getBadgeStyle(const QString& status) {
    if (status == "Active" || status == "Paid" || status == "Completed" || status == "Available" || status == "Excellent" || status == "Good") {
        return "background-color: #064E3B; color: #34D399; padding: 4px 10px; border-radius: 10px; font-weight: bold;";
    } else if (status == "Expired" || status == "Cancelled" || status == "Damaged" || status == "Inactive" || status == "Overdue") {
        return "background-color: #7F1D1D; color: #FCA5A5; padding: 4px 10px; border-radius: 10px; font-weight: bold;";
    } else if (status == "Pending" || status == "Maintenance" || status == "Partial" || status == "Fair" || status == "Scheduled") {
        return "background-color: #78350F; color: #FCD34D; padding: 4px 10px; border-radius: 10px; font-weight: bold;";
    }
    return "background-color: #334155; color: #94A3B8; padding: 4px 10px; border-radius: 10px; font-weight: bold;";
}

} // namespace FitCore
