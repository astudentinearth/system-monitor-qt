cmake_minimum_required(VERSION 3.16)

project(SystemMonitor VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(system-monitor 
    src/main.cpp
    src/MainWindow.cpp
    src/system/Sensors.cpp
    src/StatSource.cpp
)
target_link_libraries(system-monitor PRIVATE Qt6::Widgets)

