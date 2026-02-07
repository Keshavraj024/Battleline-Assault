add_library(core STATIC
    src/GameController.cpp
    src/SettingsManager.cpp
    include/GameController.h
    include/SettingsManager.h
)

target_include_directories(core PUBLIC include ${CMAKE_CURRENT_SOURCE_DIR}/src/entities/include
${CMAKE_CURRENT_SOURCE_DIR}/src/managers/include)

target_link_libraries(core PUBLIC
    Qt6::Core
    Qt6::Quick
    entities
    managers
)
