cmake_minimum_required(VERSION 3.15)
project(gameboy
    VERSION 0.1.0
    DESCRIPTION "A Game Boy emulator written in C"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()

include(cmake/CompilerWarnings.cmake)

add_library(gameboy_core STATIC
    src/cpu.c
    src/memory.c
)

target_include_directories(gameboy_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

set_project_warnings(gameboy_core)

add_executable(gameboy src/main.c)
target_link_libraries(gameboy PRIVATE gameboy_core)
set_project_warnings(gameboy)

option(GAMEBOY_BUILD_TESTS "Build unit tests" OFF)
if(GAMEBOY_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
