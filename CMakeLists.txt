cmake_minimum_required(VERSION 3.10)
project(HeartController)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)

# Hitta alla .cpp-filer i src-mappen
file(GLOB SOURCES "src/*.cpp")

# Skapa körbar fil
add_executable(heart_controller ${SOURCES})

# Viktigt: Lägg till include-mappen så att headers hittas
target_include_directories(heart_controller PRIVATE include)

# Länka med pthread
target_link_libraries(heart_controller Threads::Threads)

# Om du vill ha en finare utskrift i terminalen (valfritt)
# target_compile_options(heart_controller PRIVATE -Wall -Wextra)
