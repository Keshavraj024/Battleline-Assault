cmake_minimum_required(VERSION 3.16)

project(Battleline_Assault VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

add_subdirectory(src)

qt_add_executable(appBattleline_Assault
    main.cpp
)

qt_add_qml_module(appBattleline_Assault
    URI Battleline_Assault
    VERSION 1.0
    QML_FILES
        Main.qml
        PlayerBullet.qml
        Enemy.qml
        Player.qml
        CustomText.qml
   RESOURCES
        Assets/Enemies/AbyssalHexcrawler.png
        Assets/Enemies/AshmawSpectralite.png
        Assets/Enemies/BlazefangOverseer.png
        Assets/Enemies/CindervoidRavager.png
        Assets/Enemies/EmberclawRavager.png
        Assets/Enemies/InfernicTalonspawn.png
        Assets/Enemies/IronbaneSkitterlord.png
        Assets/Enemies/NetherfangSentinel.png
        Assets/Enemies/PyroniteWarDrone.png
        Assets/Enemies/PyrothorSovereign.png
        Assets/Enemies/VoidclawIncinerator.png
        Assets/Enemies/VoidreaverStrix.png
        Assets/Player/Skyfire.png
        Assets/Player/flame_flicker.gif
        Assets/Fonts/Doto-Bold.ttf
        Assets/Fonts/Doto-ExtraBold.ttf
        Assets/bgImg_1.png
)

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
    PUBLIC Qt6::Quick core entities
)

include(GNUInstallDirs)
install(TARGETS appBattleline_Assault
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
