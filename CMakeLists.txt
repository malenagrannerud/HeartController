
cmake_minimum_required(VERSION 3.10)
project(HeartController)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(heart_controller src/main.cpp)
target_link_libraries(heart_controller pthread)
