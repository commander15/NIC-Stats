cmake_minimum_required(VERSION 3.30)

project(NIC-Stats
    VERSION 1.0
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Looking for Qt 6
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Looking for Qxlsx
include(cmake/FindQxlsx.cmake)

qt_add_executable(App main.cpp
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
)

set_target_properties(App
    PROPERTIES
        OUTPUT_NAME NIC-Stats
)

target_link_libraries(App PRIVATE Qt::Widgets QXlsx)

# Installation

if (LINUX)
    set(RUNTIME_DEPLOY_CODE "file(COPY ${Qt6_DIR}/../../../plugins/platformthemes DESTINATION ${CMAKE_INSTALL_PREFIX}/plugins)")
elseif (WIN32)
    set(RUNTIME_DEPLOY_CODE "")
endif()

install(TARGETS App RUNTIME DESTINATION bin)

qt_generate_deploy_app_script(
    TARGET App
    OUTPUT_SCRIPT APP_DEPLOY_SCRIPT
    NO_TRANSLATIONS
)

install(SCRIPT ${APP_DEPLOY_SCRIPT})
install(CODE ${RUNTIME_DEPLOY_CODE})
