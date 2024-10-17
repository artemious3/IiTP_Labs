cmake_minimum_required(VERSION 3.26)
project(t1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
#set(CMAKE_CXX_FLAGS "-Wall -O2")
enable_testing()

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)
find_package(GTest REQUIRED)

include_directories(Date)
include_directories(app)
include_directories(app/widgets)


add_executable(t1
        main.cpp
        Date/Date.cpp
        Date/DateInfo.cpp
        app/MainWindow.cpp
        app/MainWindow.h
        app/MainWindow.ui
        app/widgets/DateTable.cpp
        app/widgets/DateTable.h
        main.cpp
)

target_link_libraries(t1
        Qt::Core
        Qt::Gui
        Qt::Widgets)

add_executable(t1_test test/main_test.cpp Date/Date.cpp Date/DateInfo.cpp)



