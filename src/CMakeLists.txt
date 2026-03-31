qt_add_library(NIC-Stats STATIC
    common/error.cpp common/error.h common/reader.cpp common/reader.h common/writer.cpp common/writer.h
    enveloppe/enveloppe.cpp enveloppe/enveloppe.h enveloppe/enveloppereader.cpp enveloppe/enveloppereader.h enveloppe/record.cpp enveloppe/record.h
    enveloppe/enveloppewriter.h enveloppe/enveloppewriter.cpp
    package/package.h package/package.cpp
    statistics/statisticscalculator.h statistics/statisticscalculator.cpp
    statistics/statisticswritter.cpp statistics/statisticswritter.h
    kit/abstractkitmanager.h kit/abstractkitmanager.cpp
    kit/kit.cpp kit/kit.h
    kit/kitmanager.h kit/kitmanager.cpp
    kit/kitsmodel.h kit/kitsmodel.cpp
    nic_stats_resources.qrc
)

target_link_libraries(NIC-Stats PRIVATE Qt::Core QXlsx)
if (Qt6Pdf_FOUND)
    target_link_libraries(NIC-Stats PUBLIC Qt::Pdf)
endif()

target_include_directories(NIC-Stats PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Installation

install(TARGETS NIC-Stats RUNTIME DESTINATION bin)
