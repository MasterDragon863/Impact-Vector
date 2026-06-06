cmake_minimum_required(VERSION 3.28)
project(ImpactVector VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ImpactVector main.cpp)


target_sources(ImpactVector PRIVATE
    src/Game.cpp
    src/Registry.cpp
    src/ECS.cpp
    src/Systems/Render.cpp
    src/Systems/Input.cpp
    src/Systems/Movement.cpp
    src/Systems/Physics.cpp
)

target_include_directories(ImpactVector PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/raylib/src
    src/
    src/Components/
)


target_link_libraries(ImpactVector PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/raylib/src/libraylib.a)
target_link_libraries(ImpactVector PRIVATE GL m pthread dl rt X11)