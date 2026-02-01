find_package(Qt6 REQUIRED COMPONENTS Multimedia)

add_library(entities STATIC
    src/Enemy.cpp
    src/Player.cpp
    src/PlayerBullet.cpp
    src/AudioManager.cpp
    include/Enemy.h
    include/Player.h
    include/PlayerBullet.h
    include/AudioManager.h 
)

target_include_directories(entities PUBLIC include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/include
)

target_link_libraries(entities PUBLIC
    Qt6::Core Qt6::Quick Qt6::Multimedia core
)
