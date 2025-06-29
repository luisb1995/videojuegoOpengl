cmake_minimum_required(VERSION 3.5)
project(juego VERSION 0.0.1)
find_package(glfw3 3.3 REQUIRED)
add_subdirectory(glad)
add_executable(juego main.cpp)
target_link_libraries(
juego PUBLIC
glfw
glad
)
