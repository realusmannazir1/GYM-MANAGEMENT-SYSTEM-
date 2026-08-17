-- FitCore Gym Management System Database Schema
-- Single Source of Truth: SQLite (fitcore.db)

PRAGMA foreign_keys = ON;

-- 1. Roles
CREATE TABLE IF NOT EXISTS roles (
    role_id INTEGER PRIMARY KEY AUTOINCREMENT,
    role_name TEXT NOT NULL UNIQUE,
    description TEXT
);

-- 2. Users / Staff
CREATE TABLE IF NOT EXISTS users (
    user_id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT NOT NULL UNIQUE,
    password_hash TEXT NOT NULL,
    salt TEXT NOT NULL,
    full_name TEXT NOT NULL,
    email TEXT,
    phone TEXT,
    role_id INTEGER NOT NULL,
    is_active INTEGER DEFAULT 1 CHECK (is_active IN (0, 1)),
    created_at TEXT DEFAULT CURRENT_TIMESTAMP,
    updated_at TEXT DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (role_id) REFERENCES roles(role_id)
);

-- 3. Members
CREATE TABLE IF NOT EXISTS members (
    member_id INTEGER PRIMARY KEY AUTOINCREMENT,
    membership_number TEXT NOT NULL UNIQUE,
    first_name TEXT NOT NULL,
    last_name TEXT NOT NULL,
    gender TEXT CHECK (gender IN ('Male', 'Female', 'Other')),
    date_of_birth TEXT,
    phone TEXT NOT NULL,
    email TEXT,
    address TEXT,
    emergency_contact TEXT,
    emergency_phone TEXT,
    registration_date TEXT NOT NULL DEFAULT (DATE('now')),
    profile_status TEXT DEFAULT 'Active' CHECK (profile_status IN ('Active', 'Inactive', 'Archived')),
    photo_path TEXT,
    notes TEXT,
    created_at TEXT DEFAULT CURRENT_TIMESTAMP,
    updated_at TEXT DEFAULT CURRENT_TIMESTAMP
);

-- 4. Membership Plans
CREATE TABLE IF NOT EXISTS membership_plans (
    plan_id INTEGER PRIMARY KEY AUTOINCREMENT,
    plan_name TEXT NOT NULL UNIQUE,
    duration_days INTEGER NOT NULL CHECK (duration_days > 0),
    price REAL NOT NULL CHECK (price >= 0),
    description TEXT,
    access_type TEXT DEFAULT 'All-Access',
    status TEXT DEFAULT 'Active' CHECK (status IN ('Active', 'Inactive')),
    created_at TEXT DEFAULT CURRENT_TIMESTAMP,
    updated_at TEXT DEFAULT CURRENT_TIMESTAMP
);

-- 5. Memberships (Historical & Current)
CREATE TABLE IF NOT EXISTS memberships (
    membership_id INTEGER PRIMARY KEY AUTOINCREMENT,
    member_id INTEGER NOT NULL,
    plan_id INTEGER NOT NULL,
    start_date TEXT NOT NULL,
    end_date TEXT NOT NULL,
    price REAL NOT NULL,
    discount REAL DEFAULT 0.0 CHECK (discount >= 0),
    final_amount REAL NOT NULL,
    status TEXT DEFAULT 'Active' CHECK (status IN ('Active', 'Expired', 'Cancelled', 'Suspended', 'Pending')),
    payment_status TEXT DEFAULT 'Paid' CHECK (payment_status IN ('Paid', 'Partial', 'Unpaid')),
    created_by INTEGER,
    created_at TEXT DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (member_id) REFERENCES members(member_id),
    FOREIGN KEY (plan_id) REFERENCES membership_plans(plan_id),
    FOREIGN KEY (created_by) REFERENCES users(user_id)
);

-- 6. Trainers
CREATE TABLE IF NOT EXISTS trainers (
    trainer_id INTEGER PRIMARY KEY AUTOINCREMENT,
    employee_number TEXT NOT NULL UNIQUE,
    first_name TEXT NOT NULL,
    last_name TEXT NOT NULL,
    phone TEXT NOT NULL,
    email TEXT,
    specialization TEXT,
    experience_years INTEGER DEFAULT 0,
    salary REAL DEFAULT 0.0,
    joining_date TEXT NOT NULL DEFAULT (DATE('now')),
    status TEXT DEFAULT 'Active' CHECK (status IN ('Active', 'Inactive', 'On Leave')),
    created_at TEXT DEFAULT CURRENT_TIMESTAMP
);

-- 7. Trainer Specializations (Junction table)
CREATE TABLE IF NOT EXISTS trainer_specializations (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    trainer_id INTEGER NOT NULL,
    specialization_name TEXT NOT NULL,
    FOREIGN KEY (trainer_id) REFERENCES trainers(trainer_id) ON DELETE CASCADE
);

-- 8. Member-Trainer Assignments
CREATE TABLE IF NOT EXISTS member_trainer_assignments (
    assignment_id INTEGER PRIMARY KEY AUTOINCREMENT,
    trainer_id INTEGER NOT NULL,
    member_id INTEGER NOT NULL,
    start_date TEXT NOT NULL DEFAULT (DATE('now')),
    end_date TEXT,
    status TEXT DEFAULT 'Active' CHECK (status IN ('Active', 'Completed', 'Terminated')),
    FOREIGN KEY (trainer_id) REFERENCES trainers(trainer_id),
    FOREIGN KEY (member_id) REFERENCES members(member_id)
);

-- 9. Personal Training Sessions
CREATE TABLE IF NOT EXISTS training_sessions (
    session_id INTEGER PRIMARY KEY AUTOINCREMENT,
    trainer_id INTEGER NOT NULL,
    member_id INTEGER NOT NULL,
    session_date TEXT NOT NULL,
    start_time TEXT NOT NULL,
    end_time TEXT NOT NULL,
    session_type TEXT DEFAULT 'Personal Training',
    notes TEXT,
    status TEXT DEFAULT 'Scheduled' CHECK (status IN ('Scheduled', 'Completed', 'Cancelled', 'Missed')),
    created_at TEXT DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (trainer_id) REFERENCES trainers(trainer_id),
    FOREIGN KEY (member_id) REFERENCES members(member_id)
);

-- 10. Exercise Library
CREATE TABLE IF NOT EXISTS exercises (
    exercise_id INTEGER PRIMARY KEY AUTOINCREMENT,
    exercise_name TEXT NOT NULL UNIQUE,
    muscle_group TEXT NOT NULL,
    equipment_required TEXT,
    difficulty TEXT CHECK (difficulty IN ('Beginner', 'Intermediate', 'Advanced')),
    instructions TEXT,
    video_url TEXT,
    status TEXT DEFAULT 'Active' CHECK (status IN ('Active', 'Inactive'))
);

-- 11. Workout Plans
CREATE TABLE IF NOT EXISTS workout_plans (
    workout_plan_id INTEGER PRIMARY KEY AUTOINCREMENT,
    member_id INTEGER NOT NULL,
    trainer_id INTEGER,
    plan_name TEXT NOT NULL,
    goal TEXT CHECK (goal IN ('Weight Loss', 'Muscle Gain', 'Strength', 'Endurance', 'General Fitness')),
    start_date TEXT NOT NULL DEFAULT (DATE('now')),
    end_date TEXT,
    notes TEXT,
    status TEXT DEFAULT 'Active' CHECK (status IN ('Active', 'Completed', 'Archived')),
    created_at TEXT DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (member_id) REFERENCES members(member_id),
    FOREIGN KEY (trainer_id) REFERENCES trainers(trainer_id)
);

-- 12. Workout Plan Exercises (Junction Table)
CREATE TABLE IF NOT EXISTS workout_plan_exercises (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    workout_plan_id INTEGER NOT NULL,
    exercise_id INTEGER NOT NULL,
    sets INTEGER NOT NULL CHECK (sets > 0),
    repetitions INTEGER NOT NULL CHECK (repetitions > 0),
    weight_kg REAL DEFAULT 0.0,
    rest_time_seconds INTEGER DEFAULT 60,
    exercise_order INTEGER DEFAULT 1,
    notes TEXT,
    FOREIGN KEY (workout_plan_id) REFERENCES workout_plans(workout_plan_id) ON DELETE CASCADE,
    FOREIGN KEY (exercise_id) REFERENCES exercises(exercise_id)
);

-- 13. Workout Logs (Completed Workouts)
CREATE TABLE IF NOT EXISTS workout_logs (
    workout_log_id INTEGER PRIMARY KEY AUTOINCREMENT,
    member_id INTEGER NOT NULL,
    workout_plan_id INTEGER,
    exercise_id INTEGER NOT NULL,
    log_date TEXT NOT NULL DEFAULT (DATE('now')),
    sets_completed INTEGER NOT NULL,
    reps_completed INTEGER NOT NULL,
    weight_used_kg REAL DEFAULT 0.0,
    duration_minutes INTEGER DEFAULT 0,
    notes TEXT,
    FOREIGN KEY (member_id) REFERENCES members(member_id),
    FOREIGN KEY (workout_plan_id) REFERENCES workout_plans(workout_plan_id),
    FOREIGN KEY (exercise_id) REFERENCES exercises(exercise_id)
);

-- 14. Member Progress / Body Measurements
CREATE TABLE IF NOT EXISTS progress_records (
    progress_id INTEGER PRIMARY KEY AUTOINCREMENT,
    member_id INTEGER NOT NULL,
    record_date TEXT NOT NULL DEFAULT (DATE('now')),
    weight_kg REAL NOT NULL CHECK (weight_kg > 0),
    height_cm REAL NOT NULL CHECK (height_cm > 0),
    bmi REAL,
    body_fat_percentage REAL,
    chest_cm REAL,
    waist_cm REAL,
    arms_cm REAL,
    thighs_cm REAL,
    shoulders_cm REAL,
    notes TEXT,
    FOREIGN KEY (member_id) REFERENCES members(member_id)
);

-- 15. Attendance
CREATE TABLE IF NOT EXISTS attendance (
    attendance_id INTEGER PRIMARY KEY AUTOINCREMENT,
    member_id INTEGER NOT NULL,
    attendance_date TEXT NOT NULL DEFAULT (DATE('now')),
    check_in_time TEXT NOT NULL DEFAULT (TIME('now', 'localtime')),
    check_out_time TEXT,
    recorded_by INTEGER,
    FOREIGN KEY (member_id) REFERENCES members(member_id),
    FOREIGN KEY (recorded_by) REFERENCES users(user_id)
);

-- 16. Payments
CREATE TABLE IF NOT EXISTS payments (
    payment_id INTEGER PRIMARY KEY AUTOINCREMENT,
    receipt_number TEXT NOT NULL UNIQUE,
    member_id INTEGER NOT NULL,
    membership_id INTEGER,
    amount REAL NOT NULL CHECK (amount >= 0),
    discount REAL DEFAULT 0.0 CHECK (discount >= 0),
    final_amount REAL NOT NULL CHECK (final_amount >= 0),
    payment_method TEXT CHECK (payment_method IN ('Cash', 'Card', 'Bank Transfer', 'Other')),
    payment_date TEXT NOT NULL DEFAULT (DATE('now')),
    received_by INTEGER,
    notes TEXT,
    FOREIGN KEY (member_id) REFERENCES members(member_id),
    FOREIGN KEY (membership_id) REFERENCES memberships(membership_id),
    FOREIGN KEY (received_by) REFERENCES users(user_id)
);

-- 17. Expenses
CREATE TABLE IF NOT EXISTS expenses (
    expense_id INTEGER PRIMARY KEY AUTOINCREMENT,
    category TEXT NOT NULL CHECK (category IN ('Electricity', 'Rent', 'Equipment', 'Maintenance', 'Cleaning', 'Staff', 'Marketing', 'Internet', 'Other')),
    description TEXT NOT NULL,
    amount REAL NOT NULL CHECK (amount > 0),
    expense_date TEXT NOT NULL DEFAULT (DATE('now')),
    payment_method TEXT CHECK (payment_method IN ('Cash', 'Card', 'Bank Transfer', 'Other')),
    added_by INTEGER,
    notes TEXT,
    FOREIGN KEY (added_by) REFERENCES users(user_id)
);

-- 18. Equipment Inventory
CREATE TABLE IF NOT EXISTS equipment (
    equipment_id INTEGER PRIMARY KEY AUTOINCREMENT,
    equipment_code TEXT NOT NULL UNIQUE,
    equipment_name TEXT NOT NULL,
    category TEXT NOT NULL,
    brand TEXT,
    model TEXT,
    purchase_date TEXT,
    purchase_price REAL DEFAULT 0.0,
    warranty_expiry TEXT,
    condition TEXT CHECK (condition IN ('Excellent', 'Good', 'Fair', 'Poor', 'Damaged')),
    status TEXT DEFAULT 'Available' CHECK (status IN ('Available', 'In Use', 'Maintenance', 'Damaged', 'Retired')),
    location TEXT DEFAULT 'Main Gym Floor',
    notes TEXT
);

-- 19. Equipment Maintenance Records
CREATE TABLE IF NOT EXISTS equipment_maintenance (
    maintenance_id INTEGER PRIMARY KEY AUTOINCREMENT,
    equipment_id INTEGER NOT NULL,
    maintenance_date TEXT NOT NULL DEFAULT (DATE('now')),
    problem_description TEXT NOT NULL,
    service_description TEXT,
    cost REAL DEFAULT 0.0 CHECK (cost >= 0),
    technician_name TEXT,
    next_maintenance_date TEXT,
    status TEXT DEFAULT 'Pending' CHECK (status IN ('Pending', 'In Progress', 'Completed', 'Cancelled')),
    FOREIGN KEY (equipment_id) REFERENCES equipment(equipment_id)
);

-- 20. Notifications
CREATE TABLE IF NOT EXISTS notifications (
    notification_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER,
    title TEXT NOT NULL,
    message TEXT NOT NULL,
    type TEXT DEFAULT 'Info' CHECK (type IN ('Info', 'Warning', 'Alert', 'Expiry', 'Payment')),
    created_at TEXT DEFAULT CURRENT_TIMESTAMP,
    read_at TEXT,
    is_read INTEGER DEFAULT 0 CHECK (is_read IN (0, 1)),
    FOREIGN KEY (user_id) REFERENCES users(user_id)
);

-- 21. Audit Logs
CREATE TABLE IF NOT EXISTS audit_logs (
    audit_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER,
    action TEXT NOT NULL,
    entity_name TEXT NOT NULL,
    entity_id INTEGER,
    description TEXT,
    timestamp TEXT DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(user_id)
);

-- 22. System Settings
CREATE TABLE IF NOT EXISTS settings (
    setting_key TEXT PRIMARY KEY,
    setting_value TEXT NOT NULL,
    description TEXT
);

-- Database Indexes for Optimization
CREATE INDEX IF NOT EXISTS idx_members_membership_number ON members(membership_number);
CREATE INDEX IF NOT EXISTS idx_members_phone ON members(phone);
CREATE INDEX IF NOT EXISTS idx_memberships_member ON memberships(member_id);
CREATE INDEX IF NOT EXISTS idx_memberships_end_date ON memberships(end_date);
CREATE INDEX IF NOT EXISTS idx_attendance_member ON attendance(member_id);
CREATE INDEX IF NOT EXISTS idx_attendance_date ON attendance(attendance_date);
CREATE INDEX IF NOT EXISTS idx_payments_member ON payments(member_id);
CREATE INDEX IF NOT EXISTS idx_payments_date ON payments(payment_date);
CREATE INDEX IF NOT EXISTS idx_training_sessions_trainer_date ON training_sessions(trainer_id, session_date);
