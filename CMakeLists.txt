# Minimum required version of CMake
cmake_minimum_required(VERSION 3.20)

# Project name and version
project(MyProject VERSION 1.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define paths for source and include directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src)

# Find all source files in the src directory
file(GLOB SRC_FILES "${SRC_DIR}/*.cpp")

# Add an executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Include directories for headers
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

# Add any necessary compiler options
# Example: Enable warnings for GCC/Clang
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Link libraries if needed (example: pthread)
# target_link_libraries(${PROJECT_NAME} PRIVATE pthread)

# Installation rules (optional)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
