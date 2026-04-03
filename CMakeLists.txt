
# ============================================================================
# CMake configuration for Heart Controller Simulation (MVC)
# ============================================================================

cmake_minimum_required(VERSION 3.10)
project(HeartController)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)

# Hitta alla .cpp-filer i alla undermappar
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(heart_controller ${SOURCES})

# Lägg till include-sökvägar för alla undermappar
target_include_directories(heart_controller PRIVATE 
    src/model
    src/view
    src/controller
)

target_link_libraries(heart_controller Threads::Threads)