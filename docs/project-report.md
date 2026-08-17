# FitCore — University Semester Project Report

## Abstract
**FitCore** is a modern desktop Gym Management System engineered in **C++17**, **Qt 6**, and **SQLite 3**. It provides a comprehensive solution for managing gym memberships, member registrations, automated attendance tracking, personal trainer schedules, workout plan generation, body measurement tracking, financial receipt generation, operational expense logging, and live database hot backup operations.

---

## 1. Project Objectives
- Build a production-grade, object-oriented desktop application using modern C++ standards.
- Enforce strict database integrity utilizing SQLite foreign keys, transactions, and indexing.
- Deliver an aesthetically rich, dark-themed Qt 6 user interface with responsive pagination, live search, and role-based access control.
- Guarantee business rule enforcement for member check-in validity, membership expiration tracking, and double-booking prevention.

---

## 2. Technical Stack
- **Programming Language**: Modern C++17 (STL, Smart Pointers, Optional, Lambda expressions)
- **GUI Framework**: Qt 6.7.2 (Core, Gui, Widgets, Sql, Charts, Svg)
- **Database Engine**: SQLite 3 (WAL Mode, Foreign Key enforcement)
- **Build System**: CMake (v3.16+) & Ninja / MinGW GCC 15.1.0

---

## 3. Core Software Modules
1. **Authentication & RBAC**: Salted SHA-256 password security with Admin, Manager, Receptionist, and Trainer roles.
2. **Member Directory**: Full member profile dashboard, quick search, status filtering, and multi-tab profile viewer.
3. **Membership Management**: Plan setup (Monthly, Quarterly, Yearly), transactional renewals, and automated expiration updates.
4. **Attendance Terminal**: Fast check-in/check-out search bar, active session tracking, and visit duration calculation.
5. **Trainer Management**: Specialization profiles, member assignments, and non-overlapping session scheduling.
6. **Workout & Exercise Engine**: Exercise library, multi-exercise workout plan builder, and workout logs.
7. **Body Progress Tracker**: Body measurement logging with automatic BMI calculation and category evaluation.
8. **Financial Management**: Payment receipt issuing (`REC-XXXXXX`), financial history log, and operational expense tracking.
9. **Reports & Data Export**: Analytical previews and CSV file export.
10. **System Maintenance & Hot Backup**: Hot SQLite snapshot backups (`VACUUM INTO`) and safe restoration with emergency rollback safeguards.

---

## 4. Algorithmic Complexities & Data Structures
- **Member Search**: \(O(N)\) filtering over cached directory vectors; \(O(\log N)\) SQLite index lookups on indexed `membership_number` and `phone`.
- **BMI Calculation**: \(O(1)\) arithmetic formula \(BMI = \frac{weight}{(height / 100)^2}\).
- **Peak Hour Frequency Analysis**: \(O(N \log N)\) aggregation map grouped by check-in hour.
- **Double-Booking Check**: \(O(K)\) interval overlap check where \(K\) is scheduled trainer sessions.

---

## 5. Verification & Testing
- **Compilation**: 100% clean compilation using GCC 15.1.0 and CMake 4.4.2.
- **Runtime Deployment**: All Qt runtime DLLs deployed using `windeployqt.exe`.
- **Transaction Verification**: Verified atomic rollback behavior when payments fail during membership renewals.

---

## 6. Conclusion
FitCore demonstrates university-level mastery of C++ OOP architecture, database design, GUI programming, and software engineering principles.
