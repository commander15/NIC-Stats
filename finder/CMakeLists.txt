qt_add_executable(Finder WIN32 main.cpp)

target_link_libraries(Finder PRIVATE Widgetry::Widgetry)

# Installation

install(TARGETS Finder RUNTIME DESTINATION bin)

qt_generate_deploy_app_script(
    TARGET Finder
    OUTPUT_SCRIPT APP_DEPLOY_SCRIPT
    NO_TRANSLATIONS
)

install(SCRIPT ${APP_DEPLOY_SCRIPT})
