cmake_minimum_required(VERSION 3.5)
project(Snake)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

add_executable(snake main.cpp)

target_link_libraries(snake ${CURSES_LIBRARIES})
