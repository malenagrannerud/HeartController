# ============================================================================
# CMake configuration for Heart Controller Simulation
# ============================================================================

cmake_minimum_required(VERSION 3.10)
project(HeartController)

# ============================================================================
# Compiler settings
# ============================================================================

# Use C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================================
# Threading support (required for terminal input)
# ============================================================================

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)

# ============================================================================
# Source files
# ============================================================================

# Find all .cpp files in src directory
file(GLOB SOURCES "src/*.cpp")

# Create executable
add_executable(heart_controller ${SOURCES})

# ============================================================================
# Include directories
# ============================================================================

# Add include directory for header files
target_include_directories(heart_controller PRIVATE include)

# ============================================================================
# Link libraries
# ============================================================================

# Link with pthread (required for termios and select)
target_link_libraries(heart_controller Threads::Threads)

# ============================================================================
# Optional compiler warnings (uncomment if desired)
# ============================================================================

# target_compile_options(heart_controller PRIVATE -Wall -Wextra)