
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

# Main executable
add_executable(process_manager ${SOURCES})

# Dependencies
find_package(spdlog REQUIRED)
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

# Link dependencies to the main executable
target_link_libraries(process_manager PRIVATE spdlog::spdlog Threads::Threads)

# Enable testing
enable_testing()

# Test executable for resource monitoring
add_executable(resource_test tests/resource_test.cpp src/data_monitoring.cpp src/logger.cpp src/thread_pool.cpp src/resource_monitoring.cpp)

# Link GTest, Threads, and spdlog to the resource_test executable
target_link_libraries(resource_test PRIVATE GTest::GTest GTest::gmock GTest::Main Threads::Threads spdlog::spdlog)

# Add test to CTest
add_test(NAME resource_test COMMAND resource_test)

