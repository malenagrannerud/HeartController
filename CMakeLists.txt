cmake_minimum_required(VERSION 3.10)
project(HeartController)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(heart_controller ${SOURCES})

target_include_directories(heart_controller PRIVATE 
    include
    include/model
    include/view
    include/controller
    src
    src/model
    src/view
    src/controller
)

target_link_libraries(heart_controller Threads::Threads)
