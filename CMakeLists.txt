cmake_minimum_required(VERSION 3.19)
project(Space_Challenge LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(Space_Challenge
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.h mainwindow.cpp mainwindow.ui
    planetswidget.h planetswidget.cpp planetswidget.ui
    rocketwidget.h rocketwidget.cpp rocketwidget.ui
    solutionwidget.h solutionwidget.cpp solutionwidget.ui
    dialogutils.h dialogutils.cpp
    core_logic/planet.h core_logic/planet.cpp
    core_logic/rocket.h core_logic/rocket.cpp
    core_logic/orbitalmath.h core_logic/orbitalmath.cpp
    core_logic/measurementunits.h core_logic/measurementunits.cpp
    solution_widgets/escapevelocitiessolutionwidget.h solution_widgets/escapevelocitiessolutionwidget.cpp solution_widgets/escapevelocitiessolutionwidget.ui
    solution_widgets/rocketaccelerationtimesolutionwidget.h solution_widgets/rocketaccelerationtimesolutionwidget.cpp solution_widgets/rocketaccelerationtimesolutionwidget.ui
    solution_widgets/simplifiedstraightjourneysolutionwidget.h solution_widgets/simplifiedstraightjourneysolutionwidget.cpp solution_widgets/simplifiedstraightjourneysolutionwidget.ui
    solution_widgets/planetrotationsolutionwidget.h solution_widgets/planetrotationsolutionwidget.cpp solution_widgets/planetrotationsolutionwidget.ui
    solution_widgets/statictransfersolutionwidget.h solution_widgets/statictransfersolutionwidget.cpp solution_widgets/statictransfersolutionwidget.ui
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
