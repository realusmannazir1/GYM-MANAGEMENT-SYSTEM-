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
    return R"(
        /* Global Window & Typography */
        QWidget {
            background-color: #F8FAFC;
            color: #0F172A;
            font-family: 'Segoe UI', 'SF Pro Text', 'Roboto', sans-serif;
            font-size: 13px;
        }

        /* Sidebar Styling (Navigation Bar - Light Blue Gradient) */
        QFrame#sidebarFrame {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #E0F2FE, stop:0.4 #DBEAFE, stop:1 #BFDBFE);
            border-right: 1px solid #93C5FD;
        }

        QListWidget#sidebarList {
            background-color: transparent;
            border: none;
            outline: none;
        }

        QListWidget#sidebarList::item {
            height: 42px;
            padding-left: 16px;
            color: #1E3A8A;
            border-radius: 8px;
            margin: 3px 8px;
            font-weight: 600;
        }

        QListWidget#sidebarList::item:hover {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 #BFDBFE, stop:1 #93C5FD);
            color: #1E3A8A;
            font-weight: 700;
        }

        QListWidget#sidebarList::item:selected {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 #2563EB, stop:1 #3B82F6);
            color: #FFFFFF;
            font-weight: 700;
        }

        /* Header Bar Styling */
        QFrame#headerFrame {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 #FFFFFF, stop:0.7 #F0F9FF, stop:1 #E0F2FE);
            border-bottom: 1px solid #CBD5E1;
        }

        QLabel#headerTitle {
            font-size: 18px;
            font-weight: bold;
            color: #0F172A;
        }

        QLabel#roleBadge {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 #2563EB, stop:1 #3B82F6);
            color: #FFFFFF;
            padding: 5px 12px;
            border-radius: 12px;
            font-size: 11px;
            font-weight: bold;
        }

        /* Stat Cards */
        QFrame#statCard {
            background-color: #FFFFFF;
            border: 1px solid #E2E8F0;
            border-radius: 12px;
            padding: 16px;
        }

        QFrame#statCard:hover {
            border: 1px solid #3B82F6;
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #FFFFFF, stop:1 #F0F9FF);
        }

        QLabel#statTitle {
            color: #64748B;
            font-size: 11px;
            font-weight: 700;
            text-transform: uppercase;
        }

        QLabel#statValue {
            color: #0F172A;
            font-size: 24px;
            font-weight: bold;
            margin-top: 4px;
        }

        /* Proper Buttons */
        QPushButton {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #3B82F6, stop:1 #2563EB);
            color: #FFFFFF;
            border: 1px solid #2563EB;
            border-radius: 6px;
            padding: 8px 16px;
            font-weight: 600;
            font-size: 13px;
        }

        QPushButton:hover {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #60A5FA, stop:1 #3B82F6);
            border: 1px solid #3B82F6;
        }

        QPushButton:pressed {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #1D4ED8, stop:1 #1E40AF);
        }

        QPushButton#secondaryBtn {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #FFFFFF, stop:1 #F1F5F9);
            color: #334155;
            border: 1px solid #CBD5E1;
        }

        QPushButton#secondaryBtn:hover {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #F1F5F9, stop:1 #E2E8F0);
            border: 1px solid #94A3B8;
            color: #0F172A;
        }

        QPushButton#dangerBtn {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #EF4444, stop:1 #DC2626);
            color: #FFFFFF;
            border: 1px solid #DC2626;
        }

        QPushButton#dangerBtn:hover {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #F87171, stop:1 #EF4444);
        }

        QPushButton#successBtn {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #10B981, stop:1 #059669);
            color: #FFFFFF;
            border: 1px solid #059669;
        }

        QPushButton#successBtn:hover {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #34D399, stop:1 #10B981);
        }

        /* Input Controls */
        QLineEdit, QComboBox, QDateEdit, QSpinBox, QDoubleSpinBox, QTextEdit {
            background-color: #FFFFFF;
            border: 1px solid #CBD5E1;
            border-radius: 6px;
            padding: 8px 12px;
            color: #0F172A;
            selection-background-color: #3B82F6;
            selection-color: #FFFFFF;
        }

        QLineEdit:focus, QComboBox:focus, QDateEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus, QTextEdit:focus {
            border: 2px solid #3B82F6;
            background-color: #F8FAFC;
        }

        /* Group Boxes */
        QGroupBox {
            background-color: #FFFFFF;
            border: 1px solid #E2E8F0;
            border-radius: 10px;
            margin-top: 14px;
            padding-top: 16px;
            font-weight: bold;
            color: #0F172A;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 3px 10px;
            background-color: #EFF6FF;
            color: #1E40AF;
            border-radius: 6px;
            border: 1px solid #BFDBFE;
        }

        /* Table Styling */
        QTableWidget {
            background-color: #FFFFFF;
            border: 1px solid #CBD5E1;
            gridline-color: #F1F5F9;
            border-radius: 8px;
            color: #0F172A;
            outline: 0;
        }

        QTableWidget::item {
            padding: 8px 12px;
            color: #1E293B;
        }

        QTableWidget::item:selected {
            background-color: #DBEAFE;
            color: #1E40AF;
        }

        QHeaderView::section {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #F8FAFC, stop:1 #E2E8F0);
            color: #334155;
            padding: 10px;
            font-weight: 700;
            font-size: 12px;
            border: none;
            border-bottom: 2px solid #CBD5E1;
        }

        /* Table Cell Action Buttons */
        QTableWidget QPushButton {
            border-radius: 6px;
            font-size: 13px;
        }

        QTableWidget QPushButton#secondaryBtn {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #FFFFFF, stop:1 #F1F5F9);
            border: 1px solid #CBD5E1;
            color: #334155;
        }

        QTableWidget QPushButton#secondaryBtn:hover {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #EFF6FF, stop:1 #DBEAFE);
            border: 1px solid #3B82F6;
            color: #1E40AF;
        }

        QTableWidget QPushButton#dangerBtn {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #FEE2E2, stop:1 #FECACA);
            border: 1px solid #FCA5A5;
            color: #991B1B;
        }

        QTableWidget QPushButton#dangerBtn:hover {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #EF4444, stop:1 #DC2626);
            border: 1px solid #DC2626;
            color: #FFFFFF;
        }

        QTableWidget QPushButton#successBtn {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #D1FAE5, stop:1 #A7F3D0);
            border: 1px solid #6EE7B7;
            color: #065F46;
        }

        QTableWidget QPushButton#successBtn:hover {
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #10B981, stop:1 #059669);
            border: 1px solid #059669;
            color: #FFFFFF;
        }

        /* Dialog Styling */
        QDialog {
            background-color: #F8FAFC;
        }

        /* Scrollbars */
        QScrollBar:vertical {
            background: #F1F5F9;
            width: 10px;
        }

        QScrollBar::handle:vertical {
            background: #CBD5E1;
            min-height: 20px;
            border-radius: 5px;
        }

        QScrollBar::handle:vertical:hover {
            background: #94A3B8;
        }

        QScrollBar:horizontal {
            background: #F1F5F9;
            height: 10px;
        }

        QScrollBar::handle:horizontal {
            background: #CBD5E1;
            min-width: 20px;
            border-radius: 5px;
        }

        QScrollBar::handle:horizontal:hover {
            background: #94A3B8;
        }
    )";
}

QString ThemeManager::getBadgeStyle(const QString& status) {
    if (status == "Active" || status == "Paid" || status == "Completed" || status == "Available" || status == "Excellent" || status == "Good") {
        return "background-color: #D1FAE5; color: #065F46; border: 1px solid #A7F3D0; padding: 4px 10px; border-radius: 10px; font-weight: bold;";
    } else if (status == "Expired" || status == "Cancelled" || status == "Damaged" || status == "Inactive" || status == "Overdue") {
        return "background-color: #FEE2E2; color: #991B1B; border: 1px solid #FCA5A5; padding: 4px 10px; border-radius: 10px; font-weight: bold;";
    } else if (status == "Pending" || status == "Maintenance" || status == "Partial" || status == "Fair" || status == "Scheduled") {
        return "background-color: #FEF3C7; color: #92400E; border: 1px solid #FDE68A; padding: 4px 10px; border-radius: 10px; font-weight: bold;";
    }
    return "background-color: #F1F5F9; color: #475569; border: 1px solid #CBD5E1; padding: 4px 10px; border-radius: 10px; font-weight: bold;";
}

} // namespace FitCore
