cmake_minimum_required(VERSION 3.16)

project(FitCore VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# Source files
set(SOURCES
    src/main.cpp
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
)

# Resources (icons/styles if present)
set(RESOURCES "")
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets/resources.qrc")
    list(APPEND RESOURCES assets/resources.qrc)
endif()

add_executable(FitCore WIN32 ${SOURCES} ${RESOURCES})

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
endif()
