include(FetchContent)

FetchContent_Declare(QXlsx
    GIT_REPOSITORY "https://github.com/AndreiCherniaev/QXlsx_fix"
    SOURCE_SUBDIR QXlsx
)

FetchContent_MakeAvailable(QXlsx)

FetchContent_GetProperties(QXlsx SOURCE_DIR QXLSX_SOURCE_DIR)
