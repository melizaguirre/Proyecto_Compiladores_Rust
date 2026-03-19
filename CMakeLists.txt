cmake_minimum_required(VERSION 3.10)
project(RustParserGUI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

add_executable(gui_app
    gui/main_gui.cpp
    gui/mainwindow.cpp
    gui/mainwindow.h
)

target_link_libraries(gui_app Qt5::Widgets)