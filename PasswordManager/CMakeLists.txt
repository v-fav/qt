cmake_minimum_required(VERSION 3.16)

project(PasswordManager VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql Network)

qt_standard_project_setup()

qt_add_executable(PasswordManager
    main.cpp
    mainwindow.h mainwindow.cpp
    mainwindow.ui
    databasemanager.h databasemanager.cpp
    passwordrepository.h passwordrepository.cpp
    passwordtablemodel.h passwordtablemodel.cpp
    passwordfilterproxymodel.h passwordfilterproxymodel.cpp
    passwordrecord.h
    passwordleakchecker.h passwordleakchecker.cpp
)

target_link_libraries(PasswordManager PRIVATE Qt6::Widgets Qt6::Sql Qt6::Network)

target_include_directories(PasswordManager PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
