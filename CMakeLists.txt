cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(rotate_rc LANGUAGES CXX)

include($ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "-Werror -Wall -Wextra -pedantic-errors -Wsign-conversion -Wconversion")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "-O3 -DNDEBUG -march=native")
endif()

set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "cmake --verbose on" FORCE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_INCLUDE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/include")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

configure_file(${PROJECT_SOURCE_DIR}/cmake/cmake.h.in ${CMAKE_INCLUDE_OUTPUT_DIRECTORY}/cmake.h @ONLY)
list(APPEND SUBROSA_DG_INCLUDE_DIRS ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

set(ROTATE_RC_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE ROTATE_RC_SOURCES CONFIGURE_DEPENDS
    ${ROTATE_RC_SOURCES_DIR}/*.cpp
)

find_path(TQDM_DIR NAMES tqdm.hpp PATHS ${PROJECT_SOURCE_DIR}/libs/tqdm-cpp)

find_path(DBG_MACRO_DIR NAMES dbg.h)

find_package(Eigen3 CONFIG REQUIRED)

find_package(fmt CONFIG REQUIRED)

find_package(OpenMP COMPONENTS CXX REQUIRED)

add_executable(${PROJECT_NAME} ${ROTATE_RC_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${TQDM_DIR})
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${DBG_MACRO_DIR})
target_link_libraries(${PROJECT_NAME} Eigen3::Eigen)
target_link_libraries(${PROJECT_NAME} fmt::fmt)
target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX)


