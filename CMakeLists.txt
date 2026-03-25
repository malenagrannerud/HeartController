
cmake_minimum_required(VERSION 3.14)
project(HeartController VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

option(BUILD_TESTS "Build unit tests" ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCES
    src/frank_starling.cpp
    src/starling_validator.cpp
    src/pressure_converter.cpp
    src/heart_model.cpp
    src/pressure_sensor.cpp
    src/motor_controller.cpp
    src/display.cpp
    src/main.cpp
)

add_executable(heart_controller ${SOURCES})
target_link_libraries(heart_controller pthread)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
