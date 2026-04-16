cmake_minimum_required(VERSION 3.20)
project(CursedTown)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML CONFIG REQUIRED COMPONENTS graphics window system)

add_executable(CursedTown
    src/main.cpp
    src/Game.cpp
    src/Player.cpp
    src/Map.cpp
    src/Zombie.cpp
    src/Projectile.cpp
    src/XPOrb.cpp
)

target_link_libraries(CursedTown PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)