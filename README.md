# FitCore — Gym Management System

FitCore is a modern desktop Gym Management System built with **C++17**, **Qt 6**, and **SQLite 3**. It features modern dark-themed GUI dashboards, comprehensive gym operations tracking, automatic membership expiry handling, financial reports, and transaction logging.

---

## 🌟 Key Features

- 🏋️ **Member Management**: Track member profiles, statuses, contact info, and active plans.
- 💳 **Membership Plans & Billing**: Auto-generate invoice receipts, manage renewals, and process plan assignments.
- ⏱️ **Attendance Tracking**: Quick check-in system with duplicate check-in prevention.
- 📊 **Dashboard & Analytics**: Real-time KPI summary cards, interactive charts, and active member statistics.
- 👨‍🏫 **Trainer Management**: Track gym trainers, assignments, and workout schedules.
- 💰 **Financial Reports & Expenses**: Expense tracking, payment histories, and revenue reports.
- 🏋️‍♂️ **Equipment & Progress Tracking**: Log gym equipment maintenance and member BMI progress logs.
- 🔔 **Automated Notifications**: Automated expiry alerts and member notifications.
- 🛡️ **Audit Logs & Live Hot Backup**: Full action audit logging and live SQLite hot backup (`VACUUM INTO`).

---

## 🛠️ Technology Stack

- **Language**: C++17
- **GUI Framework**: Qt 6.7 (Widgets, Charts, Svg, Sql)
- **Database**: SQLite 3
- **Build System**: CMake 3.16+
- **Compiler**: MinGW / GCC 64-bit

---

## 🚀 Quick Start Guide

### Prerequisites
- Qt 6.7 installed (e.g. `D:\Qt\6.7.2\mingw_64`)
- CMake 3.16+
- **MinGW-w64 GCC 13.1.0 (posix-seh, MSVCRT)** — this must match the compiler Qt 6.7.2 was built with
  (recommended location: `D:\Qt\Tools\mingw1310_64\mingw64`). Using a different GCC version
  (e.g. GCC 15 MCF) causes heap-corruption crashes because the GNU runtime DLLs are incompatible.

### 1. Build the Project
Open PowerShell in the project directory and run:

```powershell
# Configure build files (point to the Qt-matching GCC 13.1 toolchain)
cmake -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER="D:/Qt/Tools/mingw1310_64/mingw64/bin/g++.exe"

# Compile FitCore target
cmake --build build --target FitCore
```

### 2. Run the Application

Double-click [`run.bat`](run.bat) in the project root folder. It automatically
builds, deploys the matching Qt + MinGW 13.1 runtime DLLs and plugins into
`build\`, code-signs the executable (required by Windows Smart App Control),
and launches the GUI.

---

## 🔑 Demo Login Credentials

Upon launching the application, use one of the seeded accounts:

| Role | Username | Password |
| :--- | :--- | :--- |
| **Admin** | `admin` | `admin123` |
| **Manager** | `manager` | `manager123` |
| **Receptionist** | `receptionist` | `receptionist123` |
| **Trainer** | `trainer1` | `trainer123` |

---

## 📁 Project Structure

```text
GYM MANAGEMENT SYSTEM/
├── assets/                  # Icons and Qt resources (.qrc)
├── database/                # SQLite database scripts (schema.sql, seed.sql)
├── include/                 # C++ Header files
│   ├── database/            # DatabaseManager singleton
│   ├── models/              # Entity models (User, Member, Payment, etc.)
│   ├── repositories/        # Data Access Object (DAO) repositories
│   ├── services/            # Business logic services
│   ├── utils/               # ThemeManager and styling
│   └── widgets/             # Qt GUI Window and Widget headers
├── src/                     # C++ Source implementations
│   ├── database/
│   ├── models/
│   ├── repositories/
│   ├── services/
│   ├── tests/               # Test suite runner (test_main.cpp)
│   ├── utils/
│   └── widgets/             # Main dashboard, login, and tab views
├── CMakeLists.txt           # Primary CMake configuration file
├── main.cpp                 # Application entry point
├── run.bat                  # One-click Windows launch script
└── README.md                # Project documentation
```

---

## ⚡ Fast Incremental Build & Run in IDE

- Click **Run (▶️)** or press **F5** on `main.cpp` in Antigravity / VS Code.
- Uses targeted incremental compilation (`cmake --build build --target FitCore`).
- If no files changed, launching takes **< 0.2s** without rebuilding.

---

## 🧪 Running Unit Tests

To run the automated test suite:

```powershell
cmake --build build --target FitCoreTest
.\build\FitCoreTest.exe
```
