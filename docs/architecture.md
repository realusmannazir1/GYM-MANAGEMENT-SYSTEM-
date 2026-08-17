# FitCore — Architecture & Design Document

## 1. System Overview
**FitCore** is an enterprise-grade desktop Gym Management System built with modern **C++17**, **Qt 6 GUI framework**, and **SQLite 3**. It follows clean software architecture principles, separating low-level database operations from business logic and user interface layers.

```
+-------------------------------------------------------------------+
|                           Qt 6 GUI Layer                          |
|  (MainWindow, LoginWindow, Dashboard, Members, Memberships, etc.) |
+-------------------------------------------------------------------+
                                  |
                                  v
+-------------------------------------------------------------------+
|                           Service Layer                           |
|  (AuthService, MembershipService, AttendanceService, Reports)     |
+-------------------------------------------------------------------+
                                  |
                                  v
+-------------------------------------------------------------------+
|                         Repository Layer                          |
|  (UserRepository, MemberRepo, MembershipRepo, PaymentRepo, etc.)  |
+-------------------------------------------------------------------+
                                  |
                                  v
+-------------------------------------------------------------------+
|                      Database Access Layer                        |
|                     (DatabaseManager Singleton)                   |
+-------------------------------------------------------------------+
                                  |
                                  v
+-------------------------------------------------------------------+
|                        SQLite Database File                       |
|                          (data/fitcore.db)                        |
+-------------------------------------------------------------------+
```

---

## 2. Design Patterns Implemented

### 2.1 Singleton Pattern (`DatabaseManager`, `AuthenticationService`)
- **Purpose**: Ensures single thread-safe database connection instance and global session authentication state across the application lifecycle.
- **Thread Safety**: Uses C++11 static magic initialization guarantee.

### 2.2 Repository Pattern
- **Purpose**: Encapsulates data persistence and CRUD operations away from UI components.
- **Repositories**:
  - `UserRepository`
  - `MemberRepository`
  - `MembershipPlanRepository`
  - `MembershipRepository`
  - `TrainerRepository`
  - `AttendanceRepository`
  - `PaymentRepository`
  - `ExpenseRepository`
  - `WorkoutRepository`
  - `ProgressRepository`
  - `EquipmentRepository`
  - `NotificationRepository`
  - `AuditRepository`
  - `SettingsRepository`

### 2.3 Service Layer Pattern
- **Purpose**: Coordinates domain models, executes complex business rule validation (e.g., check-in eligibility, duplicate prevention), and manages multi-table SQLite transactions.
- **Services**: `AuthenticationService`, `MembershipService`, `AttendanceService`, `ReportService`, `BackupService`, `NotificationService`.

### 2.4 Qt Signal & Slot Architecture
- Enables loosely coupled event-driven communication between widgets (e.g., navigation triggers, real-time KPI re-calculations upon receipt generation).

---

## 3. Data Flow & Security Model

1. **Authentication & Hashing**:
   - Passwords stored as SHA-256 digests appended with a 16-character random salt per user.
   - Plaintext passwords never persist in DB or memory buffers.

2. **Role-Based Access Control (RBAC)**:
   - **Admin**: Full administrative privileges including backup restoration, audit log inspection, and system settings.
   - **Manager**: Full access to operational management, financial inflows, and trainer assignments.
   - **Receptionist**: Quick member check-ins, member registration, membership renewals, receipt printing.
   - **Trainer**: Dedicated access to assigned members, workout plan builder, body measurement tracker, and training calendar.

3. **Transaction Safety**:
   - Membership renewals and receipt generation execute inside explicit SQLite transactions (`BEGIN TRANSACTION`, `COMMIT`, `ROLLBACK`).
