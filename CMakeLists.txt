cmake_minimum_required(VERSION 3.16)

project(Battleline_Assault VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

# if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#     set(QT_NO_CACHEGEN ON) # Disables the QML cache generator
#     set(QT_NO_QUICK_COMPILER ON) # Disables the QML compiler
# endif()


find_package(Qt6 REQUIRED COMPONENTS Quick Multimedia)

qt_standard_project_setup(REQUIRES 6.8)

add_subdirectory(src)

set(SOURCES main.cpp)

qt_add_big_resources(SOURCES assets.qrc)
# Have big_resource.qrc treated as a source file by Qt Creator
list(APPEND SOURCES assets.qrc)
set_property(SOURCE assets.qrc PROPERTY SKIP_AUTORCC ON)

qt_add_executable(appBattleline_Assault
    ${SOURCES}
)

target_include_directories(appBattleline_Assault PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/core/include)

include(qmlmodules)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appBattleline_Assault PROPERTIES
    #    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appBattleline_Assault
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appBattleline_Assault
    PUBLIC Qt6::Quick Qt6::Multimedia
    PUBLIC core entities managers
)

include(GNUInstallDirs)
install(TARGETS appBattleline_Assault
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
