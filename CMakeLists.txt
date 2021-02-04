cmake_minimum_required (VERSION 3.8)
project(LifeGame-SFML CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(LifeGame-SFML
    src/main.cpp
    )

set(SFML_DIR "SFML/lib/cmake/SFML")

find_package(SFML COMPONENTS graphics audio REQUIRED)
target_link_libraries(LifeGame-SFML sfml-graphics sfml-window sfml-system)
