
find_package(Qt6Widgets REQUIRED)
set_package_properties(Qt5Widgets PROPERTIES TYPE REQUIRED PURPOSE "The charts example needs Qt Widgets so the Plasma desktop style works properly.")
#find_package(KF5Kirigami2 REQUIRED)
#set_package_properties(KF5Kirigami2 PROPERTIES TYPE REQUIRED PURPOSE "The charts example uses Kirigami for its UI.")
#find_package(KF5Declarative REQUIRED)
#set_package_properties(KF5Declarative PROPERTIES TYPE REQUIRED PURPOSE "The charts example uses items from KF5Declarative for its UI.")

add_executable(kquickcharts_example)

target_sources(kquickcharts_example PRIVATE
    main.cpp
    charts.qrc
)

target_link_libraries(kquickcharts_example Qt6::Quick Qt6::Widgets)

install(TARGETS kquickcharts_example DESTINATION ${KDE_INSTALL_BINDIR})
