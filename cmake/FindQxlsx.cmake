include(FetchContent)

FetchContent_Declare(QXlsx
    GIT_REPOSITORY "https://github.com/QtExcel/QXlsx"
    GIT_TAG v1.5.0
    SOURCE_SUBDIR QXlsx
)

FetchContent_MakeAvailable(QXlsx)

FetchContent_GetProperties(QXlsx SOURCE_DIR QXLSX_SOURCE_DIR)
