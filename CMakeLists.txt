cmake_minimum_required(VERSION 3.10)
project(HeartController)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/controller
    ${CMAKE_SOURCE_DIR}/src/model
    ${CMAKE_SOURCE_DIR}/src/view
)

# Source files
set(SOURCES
    src/main.cpp
    src/controller/Simulation.cpp
    src/model/Motor.cpp
    src/model/PressureSensor.cpp
    src/model/StarlingCurve.cpp
    src/model/CirculationModel.cpp
    src/model/Statistics.cpp
    src/view/Terminal.cpp
)

# Create executable
add_executable(HeartController ${SOURCES})

# Link libraries
target_link_libraries(HeartController pthread)