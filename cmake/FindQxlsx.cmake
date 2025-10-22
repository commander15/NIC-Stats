include(FetchContent)

FetchContent_Declare(QXlsx
    GIT_REPOSITORY "https://github.com/QtExcel/QXlsx"
    GIT_TAG v1.5.0
    SOURCE_SUBDIR Qxlsx
)

FetchContent_MakeAvailable(QXlsx)

FetchContent_GetProperties(QXlsx SOURCE_DIR QXLSX_SOURCE_DIR)

if (NOT TARGET QXlsx)
    add_subdirectory(${QXLSX_SOURCE_DIR}/QXlsx)
endif()
