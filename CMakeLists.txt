cmake_minimum_required(VERSION 3.19)
project(Space_Challenge LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(Space_Challenge
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.h mainwindow.cpp mainwindow.ui
    planetswidget.h planetswidget.cpp planetswidget.ui
    planet.h planet.cpp
    rocketwidget.h rocketwidget.cpp rocketwidget.ui
    rocket.h rocket.cpp
    dialogutils.h dialogutils.cpp
    solutionwidget.h solutionwidget.cpp solutionwidget.ui
)

# Needs to be added so generated UI headers see promoted widget headers
target_include_directories(Space_Challenge PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Space_Challenge
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS Space_Challenge
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Space_Challenge
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
