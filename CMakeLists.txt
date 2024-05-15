cmake_minimum_required(VERSION 3.26)

project(ledy)

# Main vars.
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Sources.
add_executable(${CMAKE_PROJECT_NAME}
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/main.h
)

# boost
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(${SRC_DIR}/platform)
add_subdirectory(${SRC_DIR}/client)
add_subdirectory(${SRC_DIR}/dsp)
add_subdirectory(${SRC_DIR}/effect)

# Standarts.
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 23)
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_EXTENSIONS OFF)

# Link
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC
    LedyPlatform
    LedyClient
    LedyDSP
    LedyEffect
)
