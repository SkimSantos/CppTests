cmake_minimum_required(VERSION 3.16)
project(CppTests)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include all header files
include_directories(${PROJECT_SOURCE_DIR}/include)

# Collect all .cpp files in src/
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Link the X11 library
find_package(X11 REQUIRED)
if (X11_FOUND)
    include_directories(
        ${PROJECT_SOURCE_DIR}/include/linux
        ${X11_INCLUDE_DIR}
    )
    add_executable(test ${SOURCES})
    target_link_libraries(test ${X11_LIBRARIES})
else()
    message(FATAL_ERROR "X11 not found")
endif()