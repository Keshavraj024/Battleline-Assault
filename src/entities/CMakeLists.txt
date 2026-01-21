add_library(entities STATIC
    Enemy.cpp
    Player.cpp
    PlayerBullet.cpp
)

target_include_directories(entities PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(entities PUBLIC
    Qt6::Core Qt6::Quick core
)
