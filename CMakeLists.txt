cmake_minimum_required(VERSION 3.30)

project(NIC-Stats
    VERSION 1.2
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Enabling C++ 20 support
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/Zc:__cplusplus)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Looking for Qt 6
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Looking for Qxlsx
include(cmake/FindQxlsx.cmake)

qt_add_executable(App WIN32
    main.cpp
    common/error.cpp common/error.h common/reader.cpp common/reader.h common/writer.cpp common/writer.h
    enveloppe/enveloppe.cpp enveloppe/enveloppe.h enveloppe/enveloppereader.cpp enveloppe/enveloppereader.h enveloppe/record.cpp enveloppe/record.h
    enveloppe/enveloppewriter.h enveloppe/enveloppewriter.cpp
    package/package.h package/package.cpp
    statistics/statisticscalculator.h statistics/statisticscalculator.cpp
    kit/abstractkitmanager.h kit/abstractkitmanager.cpp
    kit/kit.cpp kit/kit.h
    kit/kitmanager.h kit/kitmanager.cpp
    stats-wizard/statisticscomputingwizard.cpp stats-wizard/statisticscomputingwizard.h stats-wizard/statisticscomputingwizard.ui
    statistics/statisticswritter.cpp statistics/statisticswritter.h
    stats-wizard/statisticssetuppage.h stats-wizard/statisticssetuppage.cpp stats-wizard/statisticssetuppage.ui
    stats-wizard/statisticsprocessingpage.h stats-wizard/statisticsprocessingpage.cpp stats-wizard/statisticsprocessingpage.ui
    resources.qrc
    test.h test.cpp
    kit/kitsmodel.h kit/kitsmodel.cpp
    kit/kitmanagerdialog.h kit/kitmanagerdialog.cpp kit/kitmanagerdialog.ui
)

set_target_properties(App
    PROPERTIES
        OUTPUT_NAME NIC-Stats
)

target_link_libraries(App PRIVATE Qt::Widgets QXlsx)

target_compile_definitions(App PRIVATE APP_VERSION="${PROJECT_VERSION}")

# Installation

install(TARGETS App RUNTIME DESTINATION bin)

qt_generate_deploy_app_script(
    TARGET App
    OUTPUT_SCRIPT APP_DEPLOY_SCRIPT
    NO_TRANSLATIONS
)

install(SCRIPT ${APP_DEPLOY_SCRIPT})

# Package
set(CPACK_PACKAGE_NAME   "NIC-Stats")
set(CPACK_PACKAGE_VENDOR "Commander Systems")
set(CPACK_GENERATOR      ZIP)
include(CPack)

configure_file(.github/workflows/windows-build.yml.in ${CMAKE_CURRENT_SOURCE_DIR}/.github/workflows/windows-build.yml @ONLY)
