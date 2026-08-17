-- FitCore Gym Management System Seed Data

-- Enable Foreign Keys
PRAGMA foreign_keys = ON;

-- 1. Roles
INSERT OR IGNORE INTO roles (role_id, role_name, description) VALUES
(1, 'Admin', 'Full administrative access to all modules and settings'),
(2, 'Manager', 'Access to members, memberships, trainers, payments, reports, attendance, and equipment'),
(3, 'Receptionist', 'Access to member registration, check-ins, memberships, and payment recording'),
(4, 'Trainer', 'Access to assigned members, workout plans, progress tracking, and training sessions');

-- 2. Default Users (Passwords hashed using SHA-256 with salt 'FitCoreSalt2026')
-- Default Passwords:
-- admin -> 'admin123'
-- manager -> 'manager123'
-- receptionist -> 'receptionist123'
-- trainer -> 'trainer123'
-- Hash of 'admin123' + 'FitCoreSalt2026' = '240be518fabd2724ddb6f04eeb1da5967448d7e831c08c8fa822809f74c720a9'
-- Hash of 'manager123' + 'FitCoreSalt2026' = 'a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3'
-- Hash of 'receptionist123' + 'FitCoreSalt2026' = '1d871901d8ab1f810141bb0223707297ddb2f32e6522c069b2d8614cb9f67a29'
-- Hash of 'trainer123' + 'FitCoreSalt2026' = '942fa790d970e7e17c093a1d120a44f481c7f8f906f23c932a3dd737227d8847'

INSERT OR IGNORE INTO users (user_id, username, password_hash, salt, full_name, email, phone, role_id, is_active) VALUES
(1, 'admin', '240be518fabd2724ddb6f04eeb1da5967448d7e831c08c8fa822809f74c720a9', 'FitCoreSalt2026', 'System Administrator', 'admin@fitcoregym.com', '+92-300-1112233', 1, 1),
(2, 'manager', 'a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3', 'FitCoreSalt2026', 'Tariq Mahmood', 'manager@fitcoregym.com', '+92-301-2223344', 2, 1),
(3, 'receptionist', '1d871901d8ab1f810141bb0223707297ddb2f32e6522c069b2d8614cb9f67a29', 'FitCoreSalt2026', 'Ayesha Khan', 'reception@fitcoregym.com', '+92-302-3334455', 3, 1),
(4, 'trainer1', '942fa790d970e7e17c093a1d120a44f481c7f8f906f23c932a3dd737227d8847', 'FitCoreSalt2026', 'Bilal Ahmed', 'bilal.trainer@fitcoregym.com', '+92-303-4445566', 4, 1);

-- 3. System Settings
INSERT OR IGNORE INTO settings (setting_key, setting_value, description) VALUES
('gym_name', 'FitCore Gym & Fitness Club', 'Name of the fitness center'),
('address', 'Plot 45, Main Boulevard, Blue Area, Islamabad', 'Physical address'),
('phone', '+92-51-8899770', 'Contact telephone number'),
('email', 'info@fitcoregym.com', 'Official email address'),
('currency', 'PKR', 'Default system currency symbol/code'),
('currency_symbol', 'Rs.', 'Currency display prefix'),
('tax_rate_percent', '0.0', 'Sales tax percentage'),
('gym_opening_time', '06:00', 'Daily opening time'),
('gym_closing_time', '23:00', 'Daily closing time'),
('max_discount_percent', '25.0', 'Maximum allowed receptionist discount'),
('theme', 'Dark', 'UI visual theme (Dark / Light)'),
('schema_version', '1', 'Current database schema version');

-- 4. Membership Plans
INSERT OR IGNORE INTO membership_plans (plan_id, plan_name, duration_days, price, description, access_type, status) VALUES
(1, 'Monthly Basic', 30, 3500.0, 'Standard gym floor access for 30 days', 'Gym Floor', 'Active'),
(2, 'Quarterly Premium', 90, 9500.0, 'Gym floor + Cardio + Locker access for 90 days', 'All-Access', 'Active'),
(3, 'Half-Yearly Elite', 180, 17500.0, 'Full access + free trainer assessment for 6 months', 'All-Access', 'Active'),
(4, 'Yearly VIP', 365, 32000.0, 'VIP access, sauna, locker, free monthly progress consultation', 'VIP All-Access', 'Active'),
(5, 'Student Special', 30, 2500.0, 'Discounted monthly access for verified students', 'Gym Floor', 'Active');

-- 5. Exercise Library
INSERT OR IGNORE INTO exercises (exercise_id, exercise_name, muscle_group, equipment_required, difficulty, instructions) VALUES
(1, 'Barbell Bench Press', 'Chest', 'Barbell & Flat Bench', 'Intermediate', 'Lie flat on bench, lower barbell to mid-chest, press up explosively.'),
(2, 'Incline Dumbbell Press', 'Upper Chest', 'Dumbbells & Incline Bench', 'Intermediate', 'Set bench at 30 degrees, press dumbbells vertically with controlled descent.'),
(3, 'Barbell Back Squat', 'Quadriceps & Glutes', 'Barbell & Squat Rack', 'Intermediate', 'Place bar across upper traps, squat down until thighs parallel, drive through heels.'),
(4, 'Conventional Deadlift', 'Hamstrings & Back', 'Barbell & Plates', 'Advanced', 'Stand with feet shoulder-width, grip bar, keep spine neutral, extend hips and knees.'),
(5, 'Pull-Up', 'Lats & Biceps', 'Pull-Up Bar', 'Intermediate', 'Grip bar with overhand grip, pull chest up to bar, lower under control.'),
(6, 'Seated Overhead Dumbbell Press', 'Shoulders', 'Dumbbells & Adjustable Bench', 'Beginner', 'Sit upright, press dumbbells overhead until arms extended.'),
(7, 'Lat Pulldown', 'Lats', 'Cable Lat Pulldown Machine', 'Beginner', 'Grip wide bar, pull down to upper chest while engaging latissimus dorsi.'),
(8, 'Standing Barbell Bicep Curl', 'Biceps', 'EZ-Bar / Barbell', 'Beginner', 'Keep elbows fixed at sides, curl bar up toward shoulders.'),
(9, 'Triceps Pushdown', 'Triceps', 'Cable Pulley & Rope/Bar', 'Beginner', 'Keep upper arms still, push cable down until arms fully extended.'),
(10, 'Leg Press', 'Quadriceps', 'Leg Press Machine', 'Beginner', 'Place feet hip-width on platform, lower safety handles, lower weight to 90 degrees, press up.');

-- 6. Trainers
INSERT OR IGNORE INTO trainers (trainer_id, employee_number, first_name, last_name, phone, email, specialization, experience_years, salary, joining_date, status) VALUES
(1, 'TRN-000001', 'Bilal', 'Ahmed', '+92-303-4445566', 'bilal.trainer@fitcoregym.com', 'Bodybuilding & Strength', 6, 65000.0, '2024-01-15', 'Active'),
(2, 'TRN-000002', 'Usman', 'Farooq', '+92-304-5556677', 'usman.f@fitcoregym.com', 'Fat Loss & HIIT', 4, 55000.0, '2024-03-01', 'Active'),
(3, 'TRN-000003', 'Zainab', 'Malik', '+92-305-6667788', 'zainab.m@fitcoregym.com', 'CrossFit & Functional Fitness', 5, 60000.0, '2024-05-10', 'Active');

-- 7. Trainer Specializations
INSERT OR IGNORE INTO trainer_specializations (id, trainer_id, specialization_name) VALUES
(1, 1, 'Bodybuilding'), (2, 1, 'Heavy Powerlifting'),
(3, 2, 'Fat Loss'), (4, 2, 'Cardiovascular Endurance'),
(5, 3, 'CrossFit'), (6, 3, 'Mobility & Rehabilitation');

-- 8. Sample Members
INSERT OR IGNORE INTO members (member_id, membership_number, first_name, last_name, gender, date_of_birth, phone, email, address, emergency_contact, emergency_phone, registration_date, profile_status, notes) VALUES
(1, 'MEM-000001', 'Muhammad', 'Ali', 'Male', '1995-06-12', '+92-321-1234567', 'm.ali@example.com', 'F-7/2, Islamabad', 'Hamza Ali', '+92-321-7654321', '2025-08-01', 'Active', 'Goal: Weight loss and muscle definition'),
(2, 'MEM-000002', 'Ahmad', 'Shah', 'Male', '1998-11-25', '+92-322-2345678', 'ahmad.shah@example.com', 'G-11/3, Islamabad', 'Shahid Shah', '+92-322-8765432', '2025-09-15', 'Active', 'Goal: Strength training'),
(3, 'MEM-000003', 'Sara', 'Khan', 'Female', '2001-04-18', '+92-323-3456789', 'sara.k@example.com', 'I-8/4, Islamabad', 'Kamran Khan', '+92-323-9876543', '2025-10-01', 'Active', 'Goal: General fitness and endurance'),
(4, 'MEM-000004', 'Usman', 'Raza', 'Male', '1992-02-14', '+92-324-4567890', 'usman.raza@example.com', 'E-11/2, Islamabad', 'Asad Raza', '+92-324-0987654', '2025-01-10', 'Active', 'Goal: Hypertrophy'),
(5, 'MEM-000005', 'Hamza', 'Tariq', 'Male', '2003-08-30', '+92-325-5678901', 'hamza.t@example.com', 'H-12, Islamabad', 'Tariq Niaz', '+92-325-1098765', '2025-07-20', 'Active', 'Student plan user');

-- 9. Active & Historical Memberships
INSERT OR IGNORE INTO memberships (membership_id, member_id, plan_id, start_date, end_date, price, discount, final_amount, status, payment_status, created_by) VALUES
(1, 1, 2, '2026-06-01', '2026-08-30', 9500.0, 500.0, 9000.0, 'Active', 'Paid', 1),
(2, 2, 4, '2026-01-01', '2026-12-31', 32000.0, 2000.0, 30000.0, 'Active', 'Paid', 1),
(3, 3, 1, '2026-08-01', '2026-08-31', 3500.0, 0.0, 3500.0, 'Active', 'Paid', 3),
(4, 4, 1, '2026-07-01', '2026-07-31', 3500.0, 0.0, 3500.0, 'Expired', 'Paid', 3),
(5, 5, 5, '2026-08-05', '2026-09-04', 2500.0, 0.0, 2500.0, 'Active', 'Paid', 3);

-- 10. Member-Trainer Assignments
INSERT OR IGNORE INTO member_trainer_assignments (assignment_id, trainer_id, member_id, start_date, end_date, status) VALUES
(1, 1, 1, '2026-06-01', '2026-08-30', 'Active'),
(2, 2, 3, '2026-08-01', '2026-08-31', 'Active');

-- 11. Payments
INSERT OR IGNORE INTO payments (payment_id, receipt_number, member_id, membership_id, amount, discount, final_amount, payment_method, payment_date, received_by, notes) VALUES
(1, 'REC-000001', 1, 1, 9500.0, 500.0, 9000.0, 'Card', '2026-06-01', 1, 'Quarterly Premium membership payment'),
(2, 'REC-000002', 2, 2, 32000.0, 2000.0, 30000.0, 'Bank Transfer', '2026-01-01', 1, 'Yearly VIP membership payment'),
(3, 'REC-000003', 3, 3, 3500.0, 0.0, 3500.0, 'Cash', '2026-08-01', 3, 'Monthly Basic membership payment'),
(4, 'REC-000004', 4, 4, 3500.0, 0.0, 3500.0, 'Cash', '2026-07-01', 3, 'July Monthly fee'),
(5, 'REC-000005', 5, 5, 2500.0, 0.0, 2500.0, 'Cash', '2026-08-05', 3, 'Student Special fee');

-- 12. Attendance Records
INSERT OR IGNORE INTO attendance (attendance_id, member_id, attendance_date, check_in_time, check_out_time, recorded_by) VALUES
(1, 1, '2026-08-16', '07:30:00', '09:00:00', 3),
(2, 2, '2026-08-16', '18:00:00', '19:45:00', 3),
(3, 3, '2026-08-16', '17:15:00', '18:30:00', 3),
(4, 1, '2026-08-17', '07:45:00', '09:15:00', 3),
(5, 2, '2026-08-17', '18:10:00', '20:00:00', 3),
(6, 5, '2026-08-17', '16:00:00', '17:30:00', 3),
(7, 1, '2026-08-18', '07:20:00', NULL, 3),
(8, 3, '2026-08-18', '08:00:00', NULL, 3);

-- 13. Progress Records
INSERT OR IGNORE INTO progress_records (progress_id, member_id, record_date, weight_kg, height_cm, bmi, body_fat_percentage, chest_cm, waist_cm, arms_cm, thighs_cm, shoulders_cm, notes) VALUES
(1, 1, '2025-08-01', 88.5, 178.0, 27.93, 24.5, 102.0, 94.0, 36.0, 60.0, 118.0, 'Initial assessment'),
(2, 1, '2025-11-01', 84.0, 178.0, 26.51, 21.0, 104.0, 89.0, 37.5, 59.0, 120.0, '3 months progress - solid waist reduction'),
(3, 1, '2026-02-01', 80.5, 178.0, 25.41, 18.2, 106.0, 84.0, 38.5, 58.5, 122.0, '6 months progress'),
(4, 2, '2025-09-15', 75.0, 175.0, 24.49, 17.5, 98.0, 82.0, 35.0, 56.0, 114.0, 'Initial strength baseline');

-- 14. Workout Plans & Exercises
INSERT OR IGNORE INTO workout_plans (workout_plan_id, member_id, trainer_id, plan_name, goal, start_date, status, notes) VALUES
(1, 1, 1, 'Hypertrophy & Fat Cut', 'Weight Loss', '2026-06-01', 'Active', '4-day split focusing on progressive overload and elevated heart rate.');

INSERT OR IGNORE INTO workout_plan_exercises (id, workout_plan_id, exercise_id, sets, repetitions, weight_kg, rest_time_seconds, exercise_order, notes) VALUES
(1, 1, 1, 4, 10, 70.0, 90, 1, 'Warm up with empty bar first'),
(2, 1, 2, 3, 12, 24.0, 60, 2, 'Focus on chest stretch at bottom'),
(3, 1, 5, 4, 8, 0.0, 90, 3, 'Bodyweight or assisted if needed'),
(4, 1, 9, 3, 15, 30.0, 45, 4, 'Strict form, no swinging');

-- 15. Expenses
INSERT OR IGNORE INTO expenses (expense_id, category, description, amount, expense_date, payment_method, added_by, notes) VALUES
(1, 'Electricity', 'K-Electric / WAPDA monthly gym utility bill', 35000.0, '2026-08-05', 'Bank Transfer', 2, 'Peak summer AC usage'),
(2, 'Rent', 'Monthly building premises rent', 120000.0, '2026-08-01', 'Bank Transfer', 1, 'Main Commercial Building Rent'),
(3, 'Maintenance', 'Treadmill belt replacement & lubrication', 14500.0, '2026-08-10', 'Cash', 2, 'Serviced by Fitness Care Technicians'),
(4, 'Cleaning', 'Sanitization supplies and towels', 6500.0, '2026-08-12', 'Cash', 3, 'Monthly hygiene restocking');

-- 16. Equipment Inventory
INSERT OR IGNORE INTO equipment (equipment_id, equipment_code, equipment_name, category, brand, model, purchase_date, purchase_price, warranty_expiry, condition, status, location, notes) VALUES
(1, 'EQP-000001', 'Pro Commercial Treadmill T90', 'Cardio', 'LifeFitness', 'T90-Commercial', '2024-01-10', 450000.0, '2026-01-10', 'Good', 'Available', 'Cardio Zone A', 'Regular belt inspection required'),
(2, 'EQP-000002', 'Olympic Bench Press Rack', 'Free Weights', 'Hammer Strength', 'HS-BP-01', '2024-01-10', 120000.0, '2027-01-10', 'Excellent', 'Available', 'Free Weight Area', 'Heavy duty steel frame'),
(3, 'EQP-000003', 'Elliptical Trainer E-500', 'Cardio', 'Technogym', 'E-500', '2024-02-15', 380000.0, '2026-02-15', 'Fair', 'Maintenance', 'Cardio Zone B', 'Display console flickering'),
(4, 'EQP-000004', 'Dual Cable Crossover Station', 'Machines', 'Matrix', 'G7-S71', '2024-03-20', 520000.0, '2027-03-20', 'Excellent', 'Available', 'Cable Zone', 'Includes all attachment handles');

-- 17. Equipment Maintenance Records
INSERT OR IGNORE INTO equipment_maintenance (maintenance_id, equipment_id, maintenance_date, problem_description, service_description, cost, technician_name, next_maintenance_date, status) VALUES
(1, 3, '2026-08-15', 'Display console power interruption and resistance motor slip', 'Inspected main board, ordered replacement transformer', 8500.0, 'Shahid Electronics', '2026-08-22', 'In Progress');

-- 18. Initial Notifications
INSERT OR IGNORE INTO notifications (notification_id, user_id, title, message, type, is_read) VALUES
(1, 1, 'Membership Expired', 'Member Usman Raza (MEM-000004) membership expired on 2026-07-31.', 'Expiry', 0),
(2, 1, 'Maintenance Due Alert', 'Equipment Elliptical Trainer E-500 (EQP-000003) maintenance scheduled for 2026-08-22.', 'Alert', 0),
(3, 2, 'Welcome to FitCore', 'System operational with SQLite database initialized successfully.', 'Info', 1);

-- 19. Initial Audit Log
INSERT OR IGNORE INTO audit_logs (audit_id, user_id, action, entity_name, entity_id, description) VALUES
(1, 1, 'INITIALIZE_SYSTEM', 'Database', 1, 'FitCore database schema initialized and seeded with default administrative records.');
