cmake_minimum_required(VERSION 3.10)
project(HeartController VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Compiler options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
    add_compile_options(-O2 -g)
endif()

# Find required packages
find_package(Threads REQUIRED)

# ========== SOURCE FILES ==========
set(MODEL_SOURCES
    src/model/CirculationModel.cpp
    src/model/StarlingCurve.cpp
    src/model/Motor.cpp
    src/model/PressureSensor.cpp
    src/model/Statistics.cpp
)

set(CONTROLLER_SOURCES
    src/controller/Simulation.cpp
)

set(VIEW_SOURCES
    src/view/Terminal.cpp
)

set(MAIN_SOURCES
    src/main.cpp
)

# ========== MAIN EXECUTABLE ==========
add_executable(tah_simulator
    ${MAIN_SOURCES}
    ${MODEL_SOURCES}
    ${CONTROLLER_SOURCES}
    ${VIEW_SOURCES}
)

# Include directories
target_include_directories(tah_simulator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/model
    ${CMAKE_CURRENT_SOURCE_DIR}/src/controller
    ${CMAKE_CURRENT_SOURCE_DIR}/src/view
)

# Link libraries
target_link_libraries(tah_simulator
    Threads::Threads
)

# ========== UNIT TESTS ==========
option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    enable_testing()
    
    # Test executables
    add_executable(test_starling
        tests/test_starling.cpp
        src/model/StarlingCurve.cpp
    )
    
    add_executable(test_circulation
        tests/test_circulation.cpp
        src/model/CirculationModel.cpp
        src/model/StarlingCurve.cpp
    )
    
    add_executable(test_motor
        tests/test_motor.cpp
        src/model/Motor.cpp
    )
    
    add_executable(test_sensor
        tests/test_sensor.cpp
        src/model/PressureSensor.cpp
    )
    
    # Include directories for tests
    target_include_directories(test_starling PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/model)
    target_include_directories(test_circulation PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/model)
    target_include_directories(test_motor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/model)
    target_include_directories(test_sensor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/model)
    
    # Add tests to CTest
    add_test(NAME StarlingCurve COMMAND test_starling)
    add_test(NAME CirculationModel COMMAND test_circulation)
    add_test(NAME Motor COMMAND test_motor)
    add_test(NAME PressureSensor COMMAND test_sensor)
endif()

# ========== CUSTOM TARGETS (renamed to avoid conflicts) ==========
add_custom_target(run_sim
    COMMAND tah_simulator
    DEPENDS tah_simulator
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMENT "Running TAH Simulator"
)

add_custom_target(run_tests
    COMMAND ctest --output-on-failure
    DEPENDS test_starling test_circulation test_motor test_sensor
    COMMENT "Running all tests"
)

add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMENT "Cleaning build directory"
)

# Print configuration summary
message(STATUS "HeartController Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Build Tests: ${BUILD_TESTS}")
message(STATUS "  Install Prefix: ${CMAKE_INSTALL_PREFIX}")