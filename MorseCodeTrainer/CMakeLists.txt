cmake_minimum_required(VERSION 3.19)

project(MorseCodeTrainer LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql)

qt_standard_project_setup()

qt_add_executable(MorseCodeTrainer
    WIN32
    MACOSX_BUNDLE

    # Main
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    # Models
    models/testinfo.h
    models/statisticsdata.h
    models/symbol.h

    # Services
    services/morseencoder.h
    services/morseencoder.cpp
    services/settingsmanager.h services/settingsmanager.cpp
    services/testmanager.h services/testmanager.cpp

    # Widgets
    widgets/dashboardwidget.h
    widgets/dashboardwidget.cpp
    widgets/dashboardwidget.ui

    widgets/lessonselectionwidget.h
    widgets/lessonselectionwidget.cpp
    widgets/lessonselectionwidget.ui

    widgets/trainingwidget.h
    widgets/trainingwidget.cpp
    widgets/trainingwidget.ui

    widgets/referencewidget.h
    widgets/referencewidget.cpp
    widgets/referencewidget.ui

    widgets/statisticswidget.h
    widgets/statisticswidget.cpp
    widgets/statisticswidget.ui

    # Dialogs
    dialogs/settingsdialog.h
    dialogs/settingsdialog.cpp
    dialogs/settingsdialog.ui
    dialogs/sessionsummarydialog.h dialogs/sessionsummarydialog.cpp dialogs/sessionsummarydialog.ui
    dialogs/confirmresetdialog.h dialogs/confirmresetdialog.cpp dialogs/confirmresetdialog.ui
    dialogs/aboutdialog.h dialogs/aboutdialog.cpp dialogs/aboutdialog.ui

    #Data
    data/symbols.txt

    #Database
    database/DatabaseManager.h
    database/DatabaseManager.cpp
    database/databasemanager.h database/databasemanager.cpp



)

target_link_libraries(MorseCodeTrainer
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Sql
)

target_include_directories(MorseCodeTrainer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/models
    ${CMAKE_CURRENT_SOURCE_DIR}/services
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs
    ${CMAKE_CURRENT_SOURCE_DIR}/database
)

include(GNUInstallDirs)

install(TARGETS MorseCodeTrainer
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET MorseCodeTrainer
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})
