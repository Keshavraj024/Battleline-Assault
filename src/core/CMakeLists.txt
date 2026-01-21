add_library(core STATIC
    GameController.cpp
    SettingsManager.cpp
)

target_include_directories(core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(core PUBLIC
    Qt6::Core Qt6::Quick entities	
)