find_package(Qt6 REQUIRED COMPONENTS Quick)

add_library(managers STATIC
    src/EnemyManager.cpp
    include/EnemyManager.h
)

target_include_directories(managers PUBLIC include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/entities/include
)

target_link_libraries(managers PUBLIC
    Qt6::Core Qt6::Quick core
)
