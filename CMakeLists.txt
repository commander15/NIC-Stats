cmake_minimum_required(VERSION 3.30)

project(NIC-Stats
    VERSION 1.4
)

option(NIC_STATS_BUILD_FINDER "Build finder tool" OFF)

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
find_package(Qt6 REQUIRED COMPONENTS Widgets OPTIONAL_COMPONENTS Pdf)

# Looking for Qxlsx
include(cmake/FindQxlsx.cmake)

# Add library
add_subdirectory(src)

# Add finder
if (NIC_STATS_BUILD_FINDER)
    # Looking for Widgetry
    include(cmake/FindWidgetry.cmake)

    add_subdirectory(finder)
endif()

# Add app
add_subdirectory(app)

# Package
set(CPACK_PACKAGE_NAME   "NIC-Stats")
set(CPACK_PACKAGE_VENDOR "Commander Systems")
set(CPACK_GENERATOR      ZIP)
if (LINUX)
    list(APPEND CPACK_GENERATOR TGZ)
endif()
include(CPack)

# Mails
file(READ mailing-list.txt MAILING)
string(STRIP ${MAILING} MAILING)
string(REPLACE "\n" "," MAIL_TO ${MAILING})

file(READ notes/RELEASE-${PROJECT_VERSION}.md MAIL_CONTENT)
string(STRIP ${MAIL_CONTENT} MAIL_CONTENT)
string(PREPEND MAIL_CONTENT "Hello, NIC Stats ${PROJECT_VERSION} has just been released, look at the new features !\n\n")
string(APPEND MAIL_CONTENT "\n\nDownload and try: https://github.com/commander15/NIC-Stats/releases/latest")
string(REPLACE "\n" "\\n" MAIL_CONTENT ${MAIL_CONTENT})
string(REPLACE "\"" "\\\"" MAIL_CONTENT ${MAIL_CONTENT})

configure_file(.github/workflows/ci-cd.yml.in ${CMAKE_CURRENT_SOURCE_DIR}/.github/workflows/ci-cd.yml @ONLY)
