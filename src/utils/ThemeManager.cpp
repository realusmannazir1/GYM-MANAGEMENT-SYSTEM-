#include "utils/ThemeManager.h"

namespace FitCore {

QString ThemeManager::getDarkThemeQss() {
    return R"(
        /* ===================== FitCore Dark Theme ===================== */
        QWidget {
            background-color: #0F172A;
            color: #E2E8F0;
            font-family: 'Segoe UI', 'SF Pro Text', 'Roboto', sans-serif;
            font-size: 13px;
        }

        QMainWindow, QDialog { background-color: #0F172A; }

        /* Sidebar */
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
            height: 40px;
            padding-left: 14px;
            color: #94A3B8;
            border-radius: 8px;
            margin: 2px 6px;
            font-weight: 600;
        }
        QListWidget#sidebarList::item:hover { background-color: #334155; color: #F8FAFC; }
        QListWidget#sidebarList::item:selected {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #3B82F6, stop:1 #2563EB);
            color: #FFFFFF;
        }

        /* Header */
        QFrame#headerFrame { background-color: #1E293B; border-bottom: 1px solid #334155; }
        QLabel#headerTitle { font-size: 17px; font-weight: bold; color: #F8FAFC; background: transparent; }
        QLabel#roleBadge {
            background-color: #2563EB; color: #FFFFFF; padding: 4px 12px;
            border-radius: 10px; font-size: 11px; font-weight: bold;
        }

        /* Stat Cards */
        QFrame#statCard { background-color: #1E293B; border: 1px solid #334155; border-radius: 12px; }
        QFrame#statCard:hover { border: 1px solid #3B82F6; }
        QLabel#statTitle { color: #94A3B8; font-size: 11px; font-weight: 700; background: transparent; }
        QLabel#statValue { color: #F8FAFC; font-size: 24px; font-weight: bold; background: transparent; }

        /* Buttons */
        QPushButton {
            background-color: #3B82F6; color: #FFFFFF; border: 1px solid #3B82F6;
            border-radius: 6px; padding: 7px 14px; font-weight: 600; font-size: 12.5px;
        }
        QPushButton:hover { background-color: #2563EB; border-color: #2563EB; }
        QPushButton:pressed { background-color: #1D4ED8; }
        QPushButton:disabled { background-color: #334155; border-color: #334155; color: #64748B; }

        QPushButton#secondaryBtn { background-color: #334155; color: #E2E8F0; border: 1px solid #475569; }
        QPushButton#secondaryBtn:hover { background-color: #475569; border-color: #64748B; color: #FFFFFF; }

        QPushButton#dangerBtn { background-color: #EF4444; border-color: #EF4444; }
        QPushButton#dangerBtn:hover { background-color: #DC2626; }

        QPushButton#successBtn { background-color: #10B981; border-color: #10B981; }
        QPushButton#successBtn:hover { background-color: #059669; }

        QPushButton#warningBtn { background-color: #F59E0B; border-color: #F59E0B; }
        QPushButton#warningBtn:hover { background-color: #D97706; }

        /* Compact icon buttons (table action columns) */
        QPushButton#iconBtn {
            background-color: #1E293B; color: #94A3B8; border: 1px solid #334155; border-radius: 6px;
            padding: 0px; min-width: 30px; max-width: 30px; min-height: 28px; max-height: 28px;
        }
        QPushButton#iconBtn:hover { background-color: #2563EB; border-color: #3B82F6; color: #FFFFFF; }
        QPushButton#iconDangerBtn {
            background-color: #450A0A; color: #FCA5A5; border: 1px solid #7F1D1D; border-radius: 6px;
            padding: 0px; min-width: 30px; max-width: 30px; min-height: 28px; max-height: 28px;
        }
        QPushButton#iconDangerBtn:hover { background-color: #EF4444; border-color: #EF4444; color: #FFFFFF; }
        QPushButton#iconSuccessBtn {
            background-color: #064E3B; color: #6EE7B7; border: 1px solid #065F46; border-radius: 6px;
            padding: 0px; min-width: 30px; max-width: 30px; min-height: 28px; max-height: 28px;
        }
        QPushButton#iconSuccessBtn:hover { background-color: #10B981; border-color: #10B981; color: #FFFFFF; }

        /* Inputs */
        QLineEdit, QComboBox, QDateEdit, QSpinBox, QDoubleSpinBox, QTextEdit, QTimeEdit {
            background-color: #1E293B; border: 1px solid #334155; border-radius: 6px;
            padding: 6px 10px; color: #F8FAFC; selection-background-color: #2563EB; selection-color: #FFFFFF;
            min-height: 20px;
        }
        QLineEdit:focus, QComboBox:focus, QDateEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus, QTextEdit:focus, QTimeEdit:focus {
            border: 2px solid #3B82F6; background-color: #0F172A;
        }
        QComboBox::drop-down { border: none; width: 24px; }
        QComboBox QAbstractItemView {
            background-color: #1E293B; border: 1px solid #334155; border-radius: 6px;
            selection-background-color: #2563EB; selection-color: #FFFFFF; outline: none;
        }

        /* Group Box */
        QGroupBox {
            background-color: #1E293B; border: 1px solid #334155; border-radius: 10px;
            margin-top: 16px; padding-top: 12px; font-weight: bold; color: #F8FAFC;
        }
        QGroupBox::title {
            subcontrol-origin: margin; subcontrol-position: top left;
            padding: 2px 10px; color: #93C5FD; font-size: 12px;
        }

        /* Tables */
        QTableWidget, QTableView {
            background-color: #1E293B; border: 1px solid #334155; border-radius: 8px;
            gridline-color: #334155; color: #E2E8F0; outline: 0;
            alternate-background-color: #16233B; selection-background-color: #1E3A5F; selection-color: #DBEAFE;
        }
        QTableWidget::item { padding: 4px 10px; border-bottom: 1px solid #1E293B; }
        QTableWidget::item:selected { background-color: #1E3A5F; color: #DBEAFE; }
        QHeaderView::section {
            background-color: #0F172A; color: #94A3B8; padding: 8px 10px;
            font-weight: 700; font-size: 12px; border: none;
            border-bottom: 2px solid #334155; border-right: 1px solid #1E293B;
        }

        /* Tabs */
        QTabWidget::pane { border: 1px solid #334155; border-radius: 8px; background-color: #1E293B; }
        QTabBar::tab {
            background-color: #0F172A; color: #94A3B8; padding: 8px 16px; margin-right: 4px;
            border-top-left-radius: 6px; border-top-right-radius: 6px; font-weight: 600;
        }
        QTabBar::tab:selected {
            background-color: #1E293B; color: #FFFFFF; border-bottom: 3px solid #3B82F6; font-weight: 700;
        }
        QTabBar::tab:hover { background-color: #1E3A5F; }

        QDialog { background-color: #0F172A; }
        QMessageBox { background-color: #1E293B; }
        QMessageBox QLabel { color: #E2E8F0; font-size: 13px; }
        QMenu { background-color: #1E293B; border: 1px solid #334155; border-radius: 6px; padding: 4px; }
        QMenu::item { padding: 6px 20px; border-radius: 4px; }
        QMenu::item:selected { background-color: #2563EB; color: #FFFFFF; }
        QToolTip { background-color: #1E293B; color: #F8FAFC; border: 1px solid #334155; padding: 5px 8px; border-radius: 4px; }

        QScrollBar:vertical { background: transparent; width: 8px; margin: 2px; }
        QScrollBar::handle:vertical { background: #334155; min-height: 24px; border-radius: 4px; }
        QScrollBar::handle:vertical:hover { background: #475569; }
        QScrollBar:horizontal { background: transparent; height: 8px; margin: 2px; }
        QScrollBar::handle:horizontal { background: #334155; min-width: 24px; border-radius: 4px; }
        QScrollBar::handle:horizontal:hover { background: #475569; }
        QScrollBar::add-line, QScrollBar::sub-line { height: 0px; width: 0px; }
        QScrollBar::add-page, QScrollBar::sub-page { background: transparent; }

        QStatusBar { background-color: #1E293B; color: #94A3B8; border-top: 1px solid #334155; }
    )";
}

QString ThemeManager::getLightThemeQss() {
    return R"(        /* ===================== FitCore Modern Light Theme ===================== */
        QWidget {
            background-color: #F1F5F9;
            color: #0F172A;
            font-family: 'Segoe UI', 'SF Pro Text', 'Roboto', sans-serif;
            font-size: 13px;
        }

        QMainWindow, QDialog { background-color: #F1F5F9; }

        /* ---------- Sidebar ---------- */
        QFrame#sidebarFrame {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1E293B, stop:1 #0F172A);
            border-right: 1px solid #0B1220;
        }
        QFrame#sidebarFrame QPushButton {
            background-color: #1E293B; color: #E2E8F0; border: 1px solid #334155;
        }
        QFrame#sidebarFrame QPushButton:hover { background-color: #334155; border-color: #475569; color: #FFFFFF; }
        QFrame#sidebarFrame QPushButton:pressed { background-color: #0F172A; }

        QListWidget#sidebarList {
            background-color: transparent;
            border: none;
            outline: none;
            padding: 4px;
        }
        QListWidget#sidebarList::item {
            height: 40px;
            padding-left: 14px;
            color: #94A3B8;
            border-radius: 8px;
            margin: 2px 6px;
            font-weight: 600;
            font-size: 12.5px;
        }
        QListWidget#sidebarList::item:hover { background-color: #1E3A5F; color: #E2E8F0; }
        QListWidget#sidebarList::item:selected {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #3B82F6, stop:1 #2563EB);
            color: #FFFFFF;
            font-weight: 700;
        }

        /* ---------- Header ---------- */
        QFrame#headerFrame { background-color: #FFFFFF; border-bottom: 1px solid #E2E8F0; }
        QLabel#headerTitle { font-size: 17px; font-weight: bold; color: #0F172A; background: transparent; }
        QLabel#roleBadge {
            background-color: #DBEAFE; color: #1D4ED8; padding: 4px 12px; border-radius: 10px;
            font-size: 11px; font-weight: bold; border: 1px solid #BFDBFE;
        }

        /* ---------- Stat Cards ---------- */
        QFrame#statCard { background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 12px; }
        QFrame#statCard:hover { border: 1px solid #3B82F6; }
        QLabel#statTitle { color: #64748B; font-size: 11px; font-weight: 700; background: transparent; }
        QLabel#statValue { color: #0F172A; font-size: 24px; font-weight: bold; background: transparent; }

        /* ---------- Buttons ---------- */
        QPushButton {
            background-color: #3B82F6; color: #FFFFFF; border: 1px solid #3B82F6;
            border-radius: 6px; padding: 7px 14px; font-weight: 600; font-size: 12.5px;
        }
        QPushButton:hover { background-color: #2563EB; border-color: #2563EB; }
        QPushButton:pressed { background-color: #1D4ED8; }
        QPushButton:disabled { background-color: #CBD5E1; border-color: #CBD5E1; color: #64748B; }

        QPushButton#secondaryBtn { background-color: #FFFFFF; color: #334155; border: 1px solid #CBD5E1; }
        QPushButton#secondaryBtn:hover { background-color: #F1F5F9; border-color: #94A3B8; color: #0F172A; }

        QPushButton#dangerBtn { background-color: #EF4444; color: #FFFFFF; border: 1px solid #EF4444; }
        QPushButton#dangerBtn:hover { background-color: #DC2626; border-color: #DC2626; }

        QPushButton#successBtn { background-color: #10B981; color: #FFFFFF; border: 1px solid #10B981; }
        QPushButton#successBtn:hover { background-color: #059669; border-color: #059669; }

        QPushButton#warningBtn { background-color: #F59E0B; color: #FFFFFF; border: 1px solid #F59E0B; }
        QPushButton#warningBtn:hover { background-color: #D97706; border-color: #D97706; }

        /* Compact icon buttons used inside table action columns */
        QPushButton#iconBtn {
            background-color: #F8FAFC; color: #334155; border: 1px solid #CBD5E1; border-radius: 6px;
            padding: 0px; min-width: 30px; max-width: 30px; min-height: 28px; max-height: 28px;
            font-size: 13px;
        }
        QPushButton#iconBtn:hover { background-color: #EFF6FF; border-color: #3B82F6; color: #1D4ED8; }
        QPushButton#iconDangerBtn {
            background-color: #FEF2F2; color: #DC2626; border: 1px solid #FECACA; border-radius: 6px;
            padding: 0px; min-width: 30px; max-width: 30px; min-height: 28px; max-height: 28px;
            font-size: 13px;
        }
        QPushButton#iconDangerBtn:hover { background-color: #EF4444; border-color: #EF4444; color: #FFFFFF; }
        QPushButton#iconSuccessBtn {
            background-color: #ECFDF5; color: #059669; border: 1px solid #A7F3D0; border-radius: 6px;
            padding: 0px; min-width: 30px; max-width: 30px; min-height: 28px; max-height: 28px;
            font-size: 13px;
        }
        QPushButton#iconSuccessBtn:hover { background-color: #10B981; border-color: #10B981; color: #FFFFFF; }

        /* ---------- Inputs ---------- */
        QLineEdit, QComboBox, QDateEdit, QSpinBox, QDoubleSpinBox, QTextEdit, QTimeEdit {
            background-color: #FFFFFF; border: 1px solid #CBD5E1; border-radius: 6px;
            padding: 6px 10px; color: #0F172A; selection-background-color: #BFDBFE; selection-color: #1E40AF;
            min-height: 20px;
        }
        QLineEdit:focus, QComboBox:focus, QDateEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus, QTextEdit:focus, QTimeEdit:focus {
            border: 2px solid #3B82F6; background-color: #F8FAFC;
        }
        QComboBox::drop-down { border: none; width: 24px; }
        QComboBox QAbstractItemView {
            background-color: #FFFFFF; border: 1px solid #CBD5E1; border-radius: 6px;
            selection-background-color: #EFF6FF; selection-color: #1D4ED8; outline: none;
        }

        /* ---------- Group Box ---------- */
        QGroupBox {
            background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 10px;
            margin-top: 16px; padding-top: 12px; font-weight: bold; color: #0F172A;
        }
        QGroupBox::title {
            subcontrol-origin: margin; subcontrol-position: top left;
            padding: 2px 10px; color: #1D4ED8; font-size: 12px;
        }

        /* ---------- Tables ---------- */
        QTableWidget, QTableView {
            background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 8px;
            gridline-color: #F1F5F9; color: #0F172A; outline: 0;
            alternate-background-color: #F8FAFC; selection-background-color: #EFF6FF; selection-color: #1E40AF;
        }
        QTableWidget::item { padding: 4px 10px; border-bottom: 1px solid #F1F5F9; }
        QTableWidget::item:selected { background-color: #DBEAFE; color: #1E40AF; }
        QHeaderView::section {
            background-color: #F8FAFC; color: #475569; padding: 8px 10px;
            font-weight: 700; font-size: 12px; border: none;
            border-bottom: 2px solid #E2E8F0; border-right: 1px solid #F1F5F9;
        }

        /* ---------- Tabs ---------- */
        QTabWidget::pane { border: 1px solid #E2E8F0; border-radius: 8px; background-color: #FFFFFF; }
        QTabBar::tab {
            background-color: #E2E8F0; color: #64748B; padding: 8px 16px; margin-right: 4px;
            border-top-left-radius: 6px; border-top-right-radius: 6px; font-weight: 600;
        }
        QTabBar::tab:selected {
            background-color: #FFFFFF; color: #1D4ED8; border-bottom: 3px solid #3B82F6; font-weight: 700;
        }
        QTabBar::tab:hover { background-color: #DBEAFE; color: #1E40AF; }

        /* ---------- Dialog / MessageBox / Menus / Tooltips ---------- */
        QDialog { background-color: #F8FAFC; }
        QMessageBox { background-color: #F8FAFC; }
        QMessageBox QLabel { color: #0F172A; font-size: 13px; }
        QMenu { background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 6px; padding: 4px; }
        QMenu::item { padding: 6px 20px; border-radius: 4px; }
        QMenu::item:selected { background-color: #EFF6FF; color: #1D4ED8; }
        QToolTip { background-color: #0F172A; color: #F8FAFC; border: none; padding: 5px 8px; border-radius: 4px; }

        /* ---------- Scrollbars ---------- */
        QScrollBar:vertical { background: transparent; width: 8px; margin: 2px; }
        QScrollBar::handle:vertical { background: #CBD5E1; min-height: 24px; border-radius: 4px; }
        QScrollBar::handle:vertical:hover { background: #94A3B8; }
        QScrollBar:horizontal { background: transparent; height: 8px; margin: 2px; }
        QScrollBar::handle:horizontal { background: #CBD5E1; min-width: 24px; border-radius: 4px; }
        QScrollBar::handle:horizontal:hover { background: #94A3B8; }
        QScrollBar::add-line, QScrollBar::sub-line { height: 0px; width: 0px; }
        QScrollBar::add-page, QScrollBar::sub-page { background: transparent; }

        /* ---------- Status Bar ---------- */
        QStatusBar { background-color: #FFFFFF; color: #64748B; border-top: 1px solid #E2E8F0; }
        QStatusBar::item { border: none; }
    )";
}

QString ThemeManager::getBadgeStyle(const QString& status) {
    if (status == "Active" || status == "Paid" || status == "Completed" || status == "Available" || status == "Excellent" || status == "Good") {
        return "background-color: #D1FAE5; color: #065F46; border: 1px solid #A7F3D0; padding: 3px 10px; border-radius: 8px; font-weight: 600; font-size: 11px;";
    } else if (status == "Expired" || status == "Cancelled" || status == "Damaged" || status == "Inactive" || status == "Overdue") {
        return "background-color: #FEE2E2; color: #991B1B; border: 1px solid #FCA5A5; padding: 3px 10px; border-radius: 8px; font-weight: 600; font-size: 11px;";
    } else if (status == "Pending" || status == "Maintenance" || status == "Partial" || status == "Fair" || status == "Scheduled") {
        return "background-color: #FEF3C7; color: #92400E; border: 1px solid #FDE68A; padding: 3px 10px; border-radius: 8px; font-weight: 600; font-size: 11px;";
    }
    return "background-color: #F1F5F9; color: #475569; border: 1px solid #CBD5E1; padding: 3px 10px; border-radius: 8px; font-weight: 600; font-size: 11px;";
}

} // namespace FitCore