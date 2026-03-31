qt_add_executable(App WIN32
    main.cpp
    stats-wizard/statisticscomputingwizard.cpp stats-wizard/statisticscomputingwizard.h stats-wizard/statisticscomputingwizard.ui
    stats-wizard/statisticssetuppage.h stats-wizard/statisticssetuppage.cpp stats-wizard/statisticssetuppage.ui
    stats-wizard/statisticsprocessingpage.h stats-wizard/statisticsprocessingpage.cpp stats-wizard/statisticsprocessingpage.ui
    resources.qrc
    test.h test.cpp
    kit/kitmanagerdialog.h kit/kitmanagerdialog.cpp kit/kitmanagerdialog.ui
)

set_target_properties(App
    PROPERTIES
        OUTPUT_NAME NIC-Stats
)

target_link_libraries(App PRIVATE Qt::Widgets NIC-Stats)

target_compile_definitions(App PRIVATE APP_VERSION="${PROJECT_VERSION}")

# Installation

install(TARGETS App RUNTIME DESTINATION bin)

qt_generate_deploy_app_script(
    TARGET App
    OUTPUT_SCRIPT APP_DEPLOY_SCRIPT
    NO_TRANSLATIONS
)

install(SCRIPT ${APP_DEPLOY_SCRIPT})
