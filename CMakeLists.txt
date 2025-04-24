# Minimum CMake version required
cmake_minimum_required(VERSION 3.28)

# Project name and language
project(cpp_core_project LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directory
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add source files
set(SOURCES src/cpp_core.cpp src/main.cpp)

# Add executable target
add_executable(cpp_core_executable ${SOURCES})

# Link libraries (if needed, e.g., fmt)
find_package(fmt REQUIRED)
target_link_libraries(cpp_core_executable PRIVATE fmt::fmt)

# Comments: - This CMakeLists.txt sets up a project with C++23 standard. - The
# `include` directory is added to the include path. - The `src/cpp_core.cpp` and
# `main.cpp` files are compiled into the executable. - The `fmt` library is
# linked to the executable.
