cmake_minimum_required(VERSION 3.16)

project(FitCore VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Default to D:/Qt/6.7.2/mingw_64 if Qt6_DIR is not provided
if(NOT Qt6_DIR AND EXISTS "D:/Qt/6.7.2/mingw_64")
    list(APPEND CMAKE_PREFIX_PATH "D:/Qt/6.7.2/mingw_64")
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Sql Charts Svg)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/models
    ${CMAKE_CURRENT_SOURCE_DIR}/include/repositories
    ${CMAKE_CURRENT_SOURCE_DIR}/include/services
    ${CMAKE_CURRENT_SOURCE_DIR}/include/managers
    ${CMAKE_CURRENT_SOURCE_DIR}/include/database
    ${CMAKE_CURRENT_SOURCE_DIR}/include/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/include/widgets
)

add_definitions(-DQT_NO_FLOAT16)

# Source files
set(SOURCES
    main.cpp
    src/database/DatabaseManager.cpp
    src/models/User.cpp
    src/models/Member.cpp
    src/models/MembershipPlan.cpp
    src/models/Membership.cpp
    src/models/Trainer.cpp
    src/models/Attendance.cpp
    src/models/Payment.cpp
    src/models/Expense.cpp
    src/models/Exercise.cpp
    src/models/WorkoutPlan.cpp
    src/models/ProgressRecord.cpp
    src/models/Equipment.cpp
    src/models/Notification.cpp
    src/models/AuditLog.cpp
    src/repositories/UserRepository.cpp
    src/repositories/MemberRepository.cpp
    src/repositories/MembershipPlanRepository.cpp
    src/repositories/MembershipRepository.cpp
    src/repositories/TrainerRepository.cpp
    src/repositories/AttendanceRepository.cpp
    src/repositories/PaymentRepository.cpp
    src/repositories/ExpenseRepository.cpp
    src/repositories/WorkoutRepository.cpp
    src/repositories/ProgressRepository.cpp
    src/repositories/EquipmentRepository.cpp
    src/repositories/NotificationRepository.cpp
    src/repositories/AuditRepository.cpp
    src/repositories/SettingsRepository.cpp
    src/services/AuthenticationService.cpp
    src/services/MembershipService.cpp
    src/services/AttendanceService.cpp
    src/services/ReportService.cpp
    src/services/BackupService.cpp
    src/services/NotificationService.cpp
    src/utils/ThemeManager.cpp
    src/widgets/LoginWindow.cpp
    src/widgets/MainWindow.cpp
    src/widgets/DashboardWidget.cpp
    src/widgets/MembersWidget.cpp
    src/widgets/MembershipsWidget.cpp
    src/widgets/TrainersWidget.cpp
    src/widgets/AttendanceWidget.cpp
    src/widgets/PaymentsWidget.cpp
    src/widgets/WorkoutWidget.cpp
    src/widgets/ProgressWidget.cpp
    src/widgets/EquipmentWidget.cpp
    src/widgets/ExpensesWidget.cpp
    src/widgets/ReportsWidget.cpp
    src/widgets/NotificationsWidget.cpp
    src/widgets/SettingsWidget.cpp
    src/widgets/AuditLogsWidget.cpp
)

# Header files for Qt AUTOMOC
set(HEADERS
    include/database/DatabaseManager.h
    include/models/User.h
    include/models/Member.h
    include/models/MembershipPlan.h
    include/models/Membership.h
    include/models/Trainer.h
    include/models/Attendance.h
    include/models/Payment.h
    include/models/Expense.h
    include/models/Exercise.h
    include/models/WorkoutPlan.h
    include/models/ProgressRecord.h
    include/models/Equipment.h
    include/models/Notification.h
    include/models/AuditLog.h
    include/repositories/UserRepository.h
    include/repositories/MemberRepository.h
    include/repositories/MembershipPlanRepository.h
    include/repositories/MembershipRepository.h
    include/repositories/TrainerRepository.h
    include/repositories/AttendanceRepository.h
    include/repositories/PaymentRepository.h
    include/repositories/ExpenseRepository.h
    include/repositories/WorkoutRepository.h
    include/repositories/ProgressRepository.h
    include/repositories/EquipmentRepository.h
    include/repositories/NotificationRepository.h
    include/repositories/AuditRepository.h
    include/repositories/SettingsRepository.h
    include/services/AuthenticationService.h
    include/services/MembershipService.h
    include/services/AttendanceService.h
    include/services/ReportService.h
    include/services/BackupService.h
    include/services/NotificationService.h
    include/utils/ThemeManager.h
    include/widgets/LoginWindow.h
    include/widgets/MainWindow.h
    include/widgets/DashboardWidget.h
    include/widgets/MembersWidget.h
    include/widgets/MembershipsWidget.h
    include/widgets/TrainersWidget.h
    include/widgets/AttendanceWidget.h
    include/widgets/PaymentsWidget.h
    include/widgets/WorkoutWidget.h
    include/widgets/ProgressWidget.h
    include/widgets/EquipmentWidget.h
    include/widgets/ExpensesWidget.h
    include/widgets/ReportsWidget.h
    include/widgets/NotificationsWidget.h
    include/widgets/SettingsWidget.h
    include/widgets/AuditLogsWidget.h
)

# Resources (icons/styles if present)
set(RESOURCES "")
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets/resources.qrc")
    list(APPEND RESOURCES assets/resources.qrc)
endif()

add_executable(FitCore ${SOURCES} ${HEADERS} ${RESOURCES})
if(WIN32)
    set_target_properties(FitCore PROPERTIES WIN32_EXECUTABLE TRUE)
endif()


target_link_libraries(FitCore PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
    Qt6::Charts
    Qt6::Svg
)

if(MSVC)
    target_compile_options(FitCore PRIVATE /W4)
else()
    target_compile_options(FitCore PRIVATE -Wall -Wextra)
    target_link_options(FitCore PRIVATE -Wl,--allow-multiple-definition)
endif()

# Test Executable Target
set(TEST_SOURCES
    src/tests/test_main.cpp
    src/database/DatabaseManager.cpp
    src/models/User.cpp
    src/models/Member.cpp
    src/models/MembershipPlan.cpp
    src/models/Membership.cpp
    src/models/Trainer.cpp
    src/models/Attendance.cpp
    src/models/Payment.cpp
    src/models/Expense.cpp
    src/models/Exercise.cpp
    src/models/WorkoutPlan.cpp
    src/models/ProgressRecord.cpp
    src/models/Equipment.cpp
    src/models/Notification.cpp
    src/models/AuditLog.cpp
    src/repositories/UserRepository.cpp
    src/repositories/MemberRepository.cpp
    src/repositories/MembershipPlanRepository.cpp
    src/repositories/MembershipRepository.cpp
    src/repositories/TrainerRepository.cpp
    src/repositories/AttendanceRepository.cpp
    src/repositories/PaymentRepository.cpp
    src/repositories/ExpenseRepository.cpp
    src/repositories/WorkoutRepository.cpp
    src/repositories/ProgressRepository.cpp
    src/repositories/EquipmentRepository.cpp
    src/repositories/NotificationRepository.cpp
    src/repositories/AuditRepository.cpp
    src/repositories/SettingsRepository.cpp
    src/services/AuthenticationService.cpp
    src/services/MembershipService.cpp
    src/services/AttendanceService.cpp
    src/services/ReportService.cpp
    src/services/BackupService.cpp
    src/services/NotificationService.cpp
    src/utils/ThemeManager.cpp
    src/widgets/LoginWindow.cpp
    src/widgets/MainWindow.cpp
    src/widgets/DashboardWidget.cpp
    src/widgets/MembersWidget.cpp
    src/widgets/MembershipsWidget.cpp
    src/widgets/TrainersWidget.cpp
    src/widgets/AttendanceWidget.cpp
    src/widgets/PaymentsWidget.cpp
    src/widgets/WorkoutWidget.cpp
    src/widgets/ProgressWidget.cpp
    src/widgets/EquipmentWidget.cpp
    src/widgets/ExpensesWidget.cpp
    src/widgets/ReportsWidget.cpp
    src/widgets/NotificationsWidget.cpp
    src/widgets/SettingsWidget.cpp
    src/widgets/AuditLogsWidget.cpp
)

add_executable(FitCoreTest ${TEST_SOURCES} ${HEADERS})
target_link_libraries(FitCoreTest PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Sql Qt6::Charts Qt6::Svg)
if(NOT MSVC)
    target_link_options(FitCoreTest PRIVATE -Wl,--allow-multiple-definition)
endif()

# Incremental configuration test comment


