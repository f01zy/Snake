cmake_minimum_required(VERSION 3.5)
project(Snake)

include(FetchContent)
FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 3.0.1
  GIT_SHALLOW ON
  EXCLUDE_FROM_ALL SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(snake main.cpp)

target_link_libraries(snake PRIVATE SFML::Graphics)
