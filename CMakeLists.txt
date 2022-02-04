cmake_minimum_required(VERSION 3.21)
project(Platformer)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(Platformer main.cpp src/Shape.cpp)

target_link_libraries(Platformer sfml-graphics sfml-audio)