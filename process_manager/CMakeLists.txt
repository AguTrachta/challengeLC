cmake_minimum_required(VERSION 3.14)
project(HighPerformanceProcessManager VERSION 1.0 LANGUAGES CXX)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include the Conan toolchain if available
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake OPTIONAL)

# Add Conan's CMakeDeps output directory to the CMake module path
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/Release/generators")

# Include directories
include_directories(include)

# Source files
file(GLOB SOURCES "src/*.cpp")

# Add executable
add_executable(process_manager ${SOURCES})

# Find and link dependencies using CMakeDeps
find_package(spdlog REQUIRED)

# Link dependencies to the target
target_link_libraries(process_manager PRIVATE spdlog::spdlog)

# Find threading library
find_package(Threads REQUIRED)
target_link_libraries(process_manager PRIVATE Threads::Threads)

