# FitCore — User Manual & Operations Guide

## 1. Quick Start
1. Launch `build/FitCore.exe`.
2. Login screen credentials:
   - **Administrator**: `admin` / `admin123`
   - **Manager**: `manager` / `manager123`
   - **Receptionist**: `receptionist` / `receptionist123`
   - **Trainer**: `trainer1` / `trainer123`

---

## 2. Core Modules & Usage Instructions

### 2.1 Member Registration & Directory
- Click **+ Add New Member** in the Member Directory.
- Enter First Name, Last Name, Phone Number (required).
- System auto-generates a unique membership number (e.g., `MEM-000001`).
- Click **View Profile** to access the member's profile drawer (Membership history, attendance visits, receipts, body progress).

### 2.2 Membership Renewal & Transactional Receipts
- Click **Renew / Assign Membership**.
- Select Member and Plan (e.g., *Gold Annual Plan*).
- Enter discount (if applicable) and select payment method (*Cash*, *Card*, *Bank Transfer*).
- Click **Process Renewal & Receipt**. The system atomically updates the member status, extends membership validity, and issues payment receipt `REC-XXXXXX`.

### 2.3 Attendance & Fast Member Check-In Terminal
- In the top header bar or Attendance tab, enter or scan the membership code (`MEM-000001`).
- Press **Enter** or click **Check-In Member**.
- If valid, check-in time is logged immediately.
- If member profile is inactive or membership is expired, an alert banner explains the exact validation failure reason.
- Click **Check Out** when member finishes their session to record duration.

### 2.4 Personal Trainer Scheduling
- Navigate to **Trainers & PT**.
- Select trainer and click **Schedule Session**.
- System automatically checks for overlapping trainer appointments to prevent double-booking.

### 2.5 Exercise Database & Workout Plans
- Add custom exercises to the library categorized by muscle group.
- Create multi-exercise workout routines for members with prescribed sets, repetitions, weights, and rest intervals.

### 2.6 Body Composition & BMI Tracking
- Log member weight, height, and body measurements.
- System automatically calculates exact **BMI** and categorizes results into *Underweight*, *Normal*, *Overweight*, or *Obese*.

### 2.7 Financial Expenditures & Reports
- Record gym operating expenses (Rent, Utilities, Maintenance, Salaries).
- Generate analytical previews or export raw data to **CSV** format (*Members*, *Payments*, *Attendance*).

### 2.8 Database Maintenance & Hot Backup
- Navigate to **System Settings**.
- Click **Create Hot Database Backup** to generate a live snapshot `FitCore_Backup_YYYY-MM-DD_HH-MM-SS.db`.
- To restore, select a backup file. An emergency safety backup is created automatically before restoring.
